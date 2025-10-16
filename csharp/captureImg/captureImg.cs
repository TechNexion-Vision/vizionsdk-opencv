/*
captureImg.cs - Demonstrates how to use CSVizionSDK to control ISP camera brightness

Opens a video window with a live camera stream. 
Key controls:
  u : increase brightness
  d : decrease brightness
  q : stop video and quit program
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

        int minWidth = 640;
        int minHeight = 480;

        Console.WriteLine($"Video resolution: {minWidth} x {minHeight}");

        // OpenCV VideoCapture
        using var cap = new VideoCapture(devIdx, VideoCaptureAPIs.DSHOW);
        cap.Set(VideoCaptureProperties.FrameWidth, minWidth);
        cap.Set(VideoCaptureProperties.FrameHeight, minHeight);

        // Get brightness range
        int minBrightness, maxBrightness, step, def;
        CSVizionSDK.VxGetISPImageProcessingRange(cam,
            VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS,
            out minBrightness, out maxBrightness, out step, out def);

        Console.WriteLine($"Brightness min: {minBrightness}, max: {maxBrightness}, step: {step}, default: {def}");

        // Get initial brightness
        int brightness, flag;
        CSVizionSDK.VxGetISPImageProcessing(cam,
            VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS,
            out brightness, out flag);

        Console.WriteLine($"Initial ISP brightness: {brightness}, flag: {flag}");

        Console.WriteLine($"Playing video with brightness {brightness}, press 'q' to exit...");

        using var window = new Window("Adjust brightness");
        Mat frame = new Mat();

        while (true)
        {
            if (!cap.Read(frame) || frame.Empty())
            {
                Console.WriteLine("Can't receive frame (stream end?). Exiting...");
                break;
            }

            // Draw overlay labels
            DrawLabel(frame, $"Brightness: {brightness}", new Point(20, 20));
            DrawLabel(frame, "u: increase, d: decrease, q: exit", new Point(20, 40));

            window.ShowImage(frame);

            int key = Cv2.WaitKey(1) & 0xFF;
            if (key == 'u')
            {
                if (brightness < maxBrightness)
                    brightness += step;

                CSVizionSDK.VxSetISPImageProcessing(cam,
                    VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS,
                    brightness);

                Console.WriteLine($"Set brightness to: {brightness}");
            }
            else if (key == 'd')
            {
                if (brightness > minBrightness)
                    brightness -= step;

                CSVizionSDK.VxSetISPImageProcessing(cam,
                    VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS,
                    brightness);

                Console.WriteLine($"Set brightness to: {brightness}");
            }
            else if (key == 'q')
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
    }
}