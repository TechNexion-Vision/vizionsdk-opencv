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
    std::cout << "Opencv Version: " << CV_VERSION << std::endl;

    // get the camera list
    std::vector<std::string> dev_list;
    int can_num = VxDiscoverCameraDevices(dev_list);
    if (can_num == 0) {
        std::cout << "Cannot Find any Cameras!" << std::endl;
        return -1;
    }

    int dev_idx = 0;
    // initial the index 0 camera 
    std::cout << "seletected [" << dev_idx << "] device" << std::endl;
    std::cout << "device name: " << dev_list[dev_idx] << std::endl;
    auto cam = VxInitialCameraDevice(dev_idx);

    VxOpen(cam);

    // get format to min size for mjpg format
    std::vector<VxFormat> fmt_list;
    VxGetFormatList(cam, fmt_list);
    int min_width = fmt_list[0].width;
    int min_height = fmt_list[0].height;
    for (auto fmt : fmt_list) {
        // find MJPG smallest size format
        if (fmt.format == VX_IMAGE_FORMAT::MJPG &&
            fmt.width * fmt.height < min_width * min_height) {
            min_width = fmt.width;
            min_height = fmt.height;
        }
    }

    // capture the video and display with cv2.imshow()
    cv::VideoCapture cap;
    #ifdef _WIN32
        cap = cv::VideoCapture(dev_idx, cv::CAP_DSHOW);
    #else
        cap = cv::VideoCapture(dev_idx);
    #endif
    cap.set(cv::CAP_PROP_FRAME_WIDTH, min_width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, min_height);
    cv::Mat frameBefore;
    cap >> frameBefore;
    cv::imshow("Before setting", frameBefore);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cap.release();

    // get the brightness
    long value;
    int flag;
    VxGetUVCImageProcessing(cam, VX_UVC_IMAGE_PROPERTIES::UVC_IMAGE_BRIGHTNESS, value, flag);
    std::cout << "UVC brightness: " << value << std::endl;

    // set the brightness
    int result;
    result = VxSetUVCImageProcessing(cam, VX_UVC_IMAGE_PROPERTIES::UVC_IMAGE_BRIGHTNESS, 16, flag);
    std::cout << "Set UVC brightness return code: " << result << std::endl;

    // get the brightness
    VxGetUVCImageProcessing(cam, VX_UVC_IMAGE_PROPERTIES::UVC_IMAGE_BRIGHTNESS, value, flag);
    std::cout << "After setting UVC brightness: " << value << std::endl;

    // capture the video after setting the property and display with cv2.imshow()
    #ifdef _WIN32
        cap = cv::VideoCapture(dev_idx, cv::CAP_DSHOW);
    #else
        cap = cv::VideoCapture(dev_idx);
    #endif
    cap.set(cv::CAP_PROP_FRAME_WIDTH, min_width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, min_height);
    cv::Mat frameAfter;
    cap >> frameAfter;
    cv::imshow("After setting", frameAfter);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cap.release();

    VxClose(cam);
	return 0;
}