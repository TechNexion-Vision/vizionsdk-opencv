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
    int can_num = VxDiscoverCameraDevices(dev_list);
    if (can_num == 0) {
        std::cout << "Cannot Find any Cameras!" << std::endl;
        return -1;
    }

    int dev_idx = 0;
    // initial the camera
    std::cout << "seletected [" << dev_idx << "] device" << std::endl;
    std::cout << "device name: " << dev_list[dev_idx] << std::endl;
    auto cam = VxInitialCameraDevice(dev_idx);

    VxOpen(cam);
    
    // get format list
    std::vector<VxFormat> fmt_list;
    VxGetFormatList(cam, fmt_list);
    int min_width = 1920;
    int min_height = 1080;
    VxFormat mjpg_fmt;
    for (auto fmt : fmt_list) {
        // find MJPG smallest size format
        if (fmt.format == VX_IMAGE_FORMAT::MJPG &&
            fmt.width * fmt.height < min_width * min_height) {
            min_width = fmt.width;
            min_height = fmt.height;
            mjpg_fmt = fmt;
        }
    }

    // set the MJPG format
    VxSetFormat(cam, mjpg_fmt);
    // start streaming
    VxStartStreaming(cam);

    // get the MJPG format image
    uint8_t* raw_data = new uint8_t[3840 * 2160 * 2];
    int raw_size = 0;
    VxGetImage(cam, raw_data, &raw_size, 2500);

    // retrieve the data into the mat array and display with cv2.imshow()
    cv::Mat matImg;
    matImg = cv::imdecode(cv::Mat(1, raw_size, CV_8UC1, raw_data), cv::IMREAD_COLOR);
    cv::imshow("MJPG Image", matImg);
    cv::waitKey(0);
    cv::destroyAllWindows();

    VxStopStreaming(cam);
	return 0;
}