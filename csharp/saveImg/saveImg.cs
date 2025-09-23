/*
saveImg.cs - Demonstrates how to use CSVizionSDK to get the image and save the image by OpenCV.
*/

using System;
using System.Runtime.InteropServices;
using OpenCvSharp;
using VizionSDKSharp;

class SaveImg
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

        // Get available formats
        VxFormatVector fmtList = new VxFormatVector();
        CSVizionSDK.VxGetFormatList(cam, fmtList);

        int minWidth = 1920;
        int minHeight = 1080;
        VxFormat uyvyFmt = new VxFormat();

        foreach (var fmt in fmtList)
        {
            // Find minimum resolution in MJPG format
            if (fmt.format == VX_IMAGE_FORMAT.UYVY &&
                (fmt.width * fmt.height == minWidth * minHeight))
            {
                minWidth = fmt.width;
                minHeight = fmt.height;
                uyvyFmt = fmt;
            }
        }

        // Set capture format
        if (CSVizionSDK.VxSetFormat(cam, uyvyFmt) == 0)
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

        // Create Mat from raw buffer
        using var matImg = new Mat(minHeight, minWidth, MatType.CV_8UC2);

        // Copy byte array into Mat
        Marshal.Copy(buffer, 0, matImg.Data, buffer.Length);

        // Convert from UYVY to BGR
        using var bgrImg = new Mat();
        Cv2.CvtColor(matImg, bgrImg, ColorConversionCodes.YUV2BGR_UYVY);

        // Save image
        Cv2.ImWrite("capture.png", bgrImg);

        // Close camera
        CSVizionSDK.VxStopStreaming(cam);
        CSVizionSDK.VxClose(cam);

        // Cleanup
        devList.Dispose();
        fmtList.Dispose();
    }
}
