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

    // capture the video and display with cv2.imshow()
    cv::VideoCapture cap;
    cap = cv::VideoCapture(dev_idx, cv::CAP_DSHOW);
    cv::Mat frameBefore;
    cap.read(frameBefore);
    cv::namedWindow("Before setting", 0);
    cv::resizeWindow("Before setting", 640, 480);
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
    cap = cv::VideoCapture(dev_idx, cv::CAP_DSHOW);
    cv::Mat frameAfter;
    cap.read(frameAfter);
    cv::namedWindow("After setting", 0);
    cv::resizeWindow("After setting", 640, 480);
    cv::imshow("After setting", frameAfter);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cap.release();

    VxClose(cam);
	return 0;
}