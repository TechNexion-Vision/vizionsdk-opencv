/*
displayImg.cs - Demonstrates how to use CSVizionSDK to get the image and display the image by OpenCV.
*/

using System;
using System.Collections.Generic;
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

        Mat image = Cv2.ImDecode(buffer, ImreadModes.Color);

        if (image.Empty())
        {
            Console.WriteLine("Failed to decode image.");
            return;
        }

        Cv2.ImShow("MJPG Image", image);
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
