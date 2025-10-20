/*
displayImg.cpp - Demonstrates how to use vizionsdk to get the image and display the image by OpenCV.
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "VizionSDK.h"
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    // get the camera list
    std::vector<std::string> dev_list;
    int cam_num = VxDiscoverCameraDevices(dev_list);
    if (cam_num == 0) {
        std::cout << "Cannot Find any Cameras!" << std::endl;
        return -1;
    }

    // Print the device list
    std::cout << "Device List:" << std::endl;
    for (auto device : dev_list) {
        std::cout << device << std::endl;
    }

    int dev_idx = 0;
    // initial the index 0 camera
    std::cout << "seletected [" << dev_idx << "] device" << std::endl;
    std::cout << "device name: " << dev_list[dev_idx] << std::endl;
    auto cam = VxInitialCameraDevice(dev_idx);

    VxOpen(cam);

    // set format to 640*480 for UYVY or MJPG format
    VX_IMAGE_FORMAT selectFormat;
    std::cout << "Choose a format: \n1. UYVY\n2. MJPG(Default)" << std::endl;
    std::cout << "Please enter format(1 or 2): ";
    int fmtId;
    std::cin >> fmtId;

    if (fmtId == 1) {
        selectFormat = VX_IMAGE_FORMAT::UYVY;
    } else {
        selectFormat = VX_IMAGE_FORMAT::MJPG;
    }

    // set format to min size
    std::vector<VxFormat> fmt_list;
    VxGetFormatList(cam, fmt_list);
    int min_width = 640;
    int min_height = 480;
    VxFormat min_fmt;
    for (auto fmt : fmt_list) {
        if (fmt.format == selectFormat && fmt.width * fmt.height == min_width * min_height) {
            min_width = fmt.width;
            min_height = fmt.height;
            min_fmt = fmt;
        }
    }

    if (VxSetFormat(cam, min_fmt) == 0) {
        std::cout << "Set Capture Format Success!" << std::endl;
    } else {
        std::cout << "Set Capture Format Failed!" << std::endl;
    }

    // start streaming
    VxStartStreaming(cam);

    // get the min format image
    uint8_t* raw_data = new uint8_t[3840 * 2160 * 2];
    int raw_size = 0;
    VxGetImage(cam, raw_data, &raw_size, 2500);

    // retrieve the data into the mat array and display with cv2.imshow()
    cv::Mat matImg;
    if (selectFormat == VX_IMAGE_FORMAT::UYVY) {
        matImg = cv::Mat(min_height, min_width, CV_8UC2, raw_data);

        // convert from UYVY to BGR
        cv::Mat bgrImg;
        cv::cvtColor(matImg, bgrImg, cv::COLOR_YUV2BGR_UYVY);
        cv::imshow("UYVY Image", bgrImg);
    } else if (selectFormat == VX_IMAGE_FORMAT::MJPG) {
        matImg = cv::imdecode(cv::Mat(1, raw_size, CV_8UC1, raw_data), cv::IMREAD_COLOR);
        cv::imshow("MJPG Image", matImg);
    }

    cv::waitKey(0);
    cv::destroyAllWindows();

    VxStopStreaming(cam);
    VxClose(cam);
    return 0;
}