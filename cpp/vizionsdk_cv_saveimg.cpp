/* 
vizionsdk_cv_saveimg.cpp - Demonstrates how to use vizionsdk to get the image and save the image by OpenCV.
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

    // set format to 1920*1080 for mjpg format
    std::vector<VxFormat> fmt_list;
    VxGetFormatList(cam, fmt_list);
    int cap_width = 1920;
    int cap_height = 1080;
    VxFormat cap_fmt;
    for (auto fmt : fmt_list) {
        // find MJPG 1920*1080 format
        if (fmt.format == VX_IMAGE_FORMAT::MJPG &&
            fmt.width * fmt.height == cap_width * cap_height) {
            cap_width = fmt.width;
            cap_height = fmt.height;
            cap_fmt = fmt;
        }
    }

    if (VxSetFormat(cam, cap_fmt) == 0) {
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

    // retrieve the data into the mat array and save with cv2.imwrite()
    cv::Mat matImg;
    matImg = cv::imdecode(cv::Mat(1, raw_size, CV_8UC1, raw_data), cv::IMREAD_COLOR);
    cv::imwrite("capture.png", matImg);

    VxStopStreaming(cam);
    VxClose(cam);
	return 0;
}