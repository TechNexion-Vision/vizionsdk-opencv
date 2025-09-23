/*
setehdr.cs - Demonstrates how to use CSVizionSDK to control ISP camera brightness

Opens a video window with live camera stream.  In the window, a keys can be pressed to
increase or decrease the number of exposures used for eHDR.

Reads the the exposure time and gain, and displays on video window.

This script only works with cameras that have eHDR functionality:
 AR0821
 AR0822

Keys:
 e: increase the maximum number of exposures for eHDR (from 1 to 4 depending on the camera).
    When it reaches the maximum, it rolls over to 1 exposure.
 q: stop playing video and quit program
*/

using System;
using System.Collections.Generic;
using OpenCvSharp;
using VizionSDKSharp;

class CaptureImg
{
    static void DrawLabel(Mat img, string text, Point pos)
    {
        HersheyFonts fontFace = HersheyFonts.HersheySimplex;
        double fontScale = 0.5;
        Scalar color = Scalar.White;
        int thickness = -1; // filled rectangle
        int margin = 5;

        // Measure text size
        int baseline;
        var textSize = Cv2.GetTextSize(text, fontFace, fontScale, 1, out baseline);

        int startX = pos.X - margin;
        int startY = pos.Y + margin;
        int endX = pos.X + textSize.Width + margin;
        int endY = pos.Y - textSize.Height - margin;

        // Draw background rectangle
        Cv2.Rectangle(img, new Point(startX, startY), new Point(endX, endY), Scalar.Black, thickness);

        // Draw text
        Cv2.PutText(img, text, pos, fontFace, fontScale, color, 1, LineTypes.AntiAlias);
    }

    static void Main()
    {
        // Discover camera devices
        StringVector devList = new StringVector();
        int camCount = CSVizionSDK.VxDiscoverCameraDevices(devList);
        if (camCount == 0)
        {
            Console.WriteLine("Cannot find any cameras!");
            return;
        }

        Console.WriteLine("Device List:");
        for (int i = 0; i < devList.Count; i++)
        {
            Console.WriteLine($"[{i}] {devList[i]}");
        }

        // Select first camera
        int devIdx = 0;
        Console.WriteLine($"Selected [{devIdx}] device");
        Console.WriteLine($"Device name: {devList[devIdx]}");

        var cam = CSVizionSDK.VxInitialCameraDevice(devIdx);

        // Open camera
        CSVizionSDK.VxOpen(cam);

        // Get available formats
        VxFormatVector fmtList = new VxFormatVector();
        CSVizionSDK.VxGetFormatList(cam, fmtList);

        int minWidth = 1920;
        int minHeight = 1080;
        VxFormat mjpgFmt = new VxFormat();

        foreach (var fmt in fmtList)
        {
            // Find minimum resolution in MJPG format
            if (fmt.format == VX_IMAGE_FORMAT.MJPG &&
                (fmt.width * fmt.height < minWidth * minHeight))
            {
                minWidth = fmt.width;
                minHeight = fmt.height;
                mjpgFmt = fmt;
            }
        }

        // Set capture format
        if (CSVizionSDK.VxSetFormat(cam, mjpgFmt) == 0)
            Console.WriteLine("Set Capture Format Success!");
        else
            Console.WriteLine("Set Capture Format Failed!");

        Console.WriteLine($"Video resolution: {minWidth} x {minHeight}");

        // OpenCV VideoCapture
        using var cap = new VideoCapture(devIdx, VideoCaptureAPIs.DSHOW);
        cap.Set(VideoCaptureProperties.FrameWidth, minWidth);
        cap.Set(VideoCaptureProperties.FrameHeight, minHeight);

        // Get ehdr info
        int minehdr, maxehdr, step, def;
        CSVizionSDK.VxGetISPImageProcessingRange(cam,
            VX_ISP_IMAGE_PROPERTIES.ISP_EHDR_EXPOSURE_MAX_NUMBER,
            out minehdr, out maxehdr, out step, out def);

        Console.WriteLine($"eHDR min: {minehdr}, max: {maxehdr}, step: {step}, default: {def}");

        int ehdr_exposures = def;

        Console.WriteLine($"Playing video with eHDR exposure = {ehdr_exposures}, press 'q' to exit...");

        using var window = new Window("Adjust eHDR");
        Mat frame = new Mat();
        byte gain;
        uint exposure;

        while (true)
        {
            if (!cap.Read(frame) || frame.Empty())
            {
                Console.WriteLine("Can't receive frame (stream end?). Exiting...");
                break;
            }

            CSVizionSDK.VxGetCurrentGain(cam, out gain);
            CSVizionSDK.VxGetCurrentExposure(cam, out exposure);

            // Draw overlay labels
            DrawLabel(frame, $"eHDR exposures: {ehdr_exposures}", new Point(20, 60));
            DrawLabel(frame, "e: increase exposures, q: exit", new Point(20, 20));
            DrawLabel(frame, $"gain: {gain}", new Point(20, 80));
            DrawLabel(frame, $"exposure (us): {exposure}", new Point(20, 100));

            window.ShowImage(frame);

            int key = Cv2.WaitKey(1) & 0xFF;
            if (key == 'u')
            {
                if (ehdr_exposures < maxehdr)
                {
                    ehdr_exposures += step;
                } else
                {
                    ehdr_exposures = minehdr;
                }
                    
                CSVizionSDK.VxSetISPImageProcessing(cam,
                    VX_ISP_IMAGE_PROPERTIES.ISP_EHDR_EXPOSURE_MAX_NUMBER,
                    ehdr_exposures);

                Console.WriteLine($"Set max eHDR exposures to: {ehdr_exposures}");
            }
            if (key == 'q')
            {
                break;
            }
        }

        cap.Release();
        Cv2.DestroyAllWindows();

        // Close camera
        CSVizionSDK.VxClose(cam);

        // Cleanup
        devList.Dispose();
        fmtList.Dispose();
    }
}