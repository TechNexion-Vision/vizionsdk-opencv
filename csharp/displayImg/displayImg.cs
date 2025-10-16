/*
displayImg.cs - Demonstrates how to use CSVizionSDK to get the image and display the image by OpenCV.
*/

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using OpenCvSharp;
using VizionSDKSharp;

class DisplayImg
{
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

        Console.WriteLine("Choose a format:\n1. UYVY\n2. MJPG(Default)");
        Console.Write("Please enter format (1 or 2): ");
        int fmtId = int.Parse(Console.ReadLine() ?? "1");

        VX_IMAGE_FORMAT selectFormat = (fmtId == 1)
            ? VX_IMAGE_FORMAT.UYVY
            : VX_IMAGE_FORMAT.MJPG;

        // Get available formats
        VxFormatVector fmtList = new VxFormatVector();
        CSVizionSDK.VxGetFormatList(cam, fmtList);

        int minWidth = 640;
        int minHeight = 480;
        VxFormat minFmt = new VxFormat();

        foreach (var fmt in fmtList)
        {
            // Find minimum resolution in UYVY/MJPG format
            if (fmt.format == selectFormat &&
                (fmt.width * fmt.height == minWidth * minHeight))
            {
                minWidth = fmt.width;
                minHeight = fmt.height;
                minFmt = fmt;
            }
        }

        // Set capture format
        if (CSVizionSDK.VxSetFormat(cam, minFmt) == 0)
            Console.WriteLine("Set Capture Format Success!");
        else
            Console.WriteLine("Set Capture Format Failed!");

        // Start streaming
        CSVizionSDK.VxStartStreaming(cam);

        // Get image
        int dataSize;
        byte[] buffer = new byte[minWidth * minHeight * 2];
        VX_CAPTURE_RESULT result = CSVizionSDK.VxGetImage(cam, buffer, out dataSize, 3000);
        if (result != VX_CAPTURE_RESULT.VX_SUCCESS)
        {
            Console.WriteLine("fail");
        }
        else
        {
            Console.WriteLine($"Get Image: {result} / {dataSize}");
        }

       if (selectFormat == VX_IMAGE_FORMAT.UYVY)
        {
            // Create Mat from raw buffer
            using var matImg = new Mat(minHeight, minWidth, MatType.CV_8UC2);

            // Copy byte array into Mat
            Marshal.Copy(buffer, 0, matImg.Data, buffer.Length);

            // Convert from UYVY to BGR
            Mat bgrImg = new Mat();
            Cv2.CvtColor(matImg, bgrImg, ColorConversionCodes.YUV2BGR_UYVY);
            Cv2.ImShow("UYVY Image", bgrImg);
        }
        else if (selectFormat == VX_IMAGE_FORMAT.MJPG)
        {
            // Create Mat from raw buffer
            using var matImg = new Mat(1, buffer.Length, MatType.CV_8UC1);

            // Copy byte array into Mat
            Marshal.Copy(buffer, 0, matImg.Data, buffer.Length);
            using var decoded = Cv2.ImDecode(matImg, ImreadModes.Color);
            Cv2.ImShow("MJPG Image", decoded);
        }

        Cv2.WaitKey(0);
        Cv2.DestroyAllWindows();

        // Close camera
        CSVizionSDK.VxStopStreaming(cam);
        CSVizionSDK.VxClose(cam);

        // Cleanup
        devList.Dispose();
        fmtList.Dispose();
    }
}
