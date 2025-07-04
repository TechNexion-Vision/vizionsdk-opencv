/*
vizionsdkcapture.cpp - Demonstrates how to use vizionsdk to control ISP camera brightness

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

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "VizionSDK.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

void draw_label(cv::Mat& img, const std::string& text, const cv::Point& pos) {
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.5;
    cv::Scalar color = cv::Scalar::all(255);
    int thickness = cv::FILLED;
    int margin = 5;
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, font_face, scale, thickness, &baseline);

    int end_x = pos.x + textSize.width + margin;
    int end_y = pos.y - textSize.height - margin;
    int start_x = pos.x - margin;
    int start_y = pos.y + margin;

    cv::rectangle(img, cv::Point(start_x, start_y), cv::Point(end_x, end_y), cv::Scalar(0, 0, 0),
                  thickness);
    cv::putText(img, text, pos, font_face, scale, color, 1, cv::LINE_AA);
}

int main() {
    // Get the camera list
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

    // Choose the first camera device
    int dev_idx = 0;
    std::cout << "seletected [" << dev_idx << "] device" << std::endl;
    std::cout << "device name: " << dev_list[dev_idx] << std::endl;
    auto cam = VxInitialCameraDevice(dev_idx);

    // Open camera
    VxOpen(cam);

    // Get format
    std::vector<VxFormat> fmt_list;
    VxGetFormatList(cam, fmt_list);

    int min_width = 1920;
    int min_height = 1080;
    VxFormat mjpg_fmt;
    for (auto fmt : fmt_list) {
        // Find the minimum resolution in MJPG format
        if (fmt.format == VX_IMAGE_FORMAT::MJPG &&
            fmt.width * fmt.height < min_width * min_height) {
            min_width = fmt.width;
            min_height = fmt.height;
            mjpg_fmt = fmt;
        }
    }

    // Set format
    if (VxSetFormat(cam, mjpg_fmt) == 0) {
        std::cout << "Set Capture Format Success!" << std::endl;
    } else {
        std::cout << "Set Capture Format Failed!" << std::endl;
    }

    std::cout << "Video resolution: " << min_width << " x " << min_height << std::endl;

    // Capture image with opencv VideoCapture API
    cv::VideoCapture cap;
#ifdef _WIN32
    cap = cv::VideoCapture(dev_idx, cv::CAP_DSHOW);
#else
    cap = cv::VideoCapture(dev_idx);
#endif

    cap.set(cv::CAP_PROP_FRAME_WIDTH, min_width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, min_height);

    // Get the eHDR info
    int min_ehdr, max_ehdr, step, def;
    VxGetISPImageProcessingRange(cam, VX_ISP_IMAGE_PROPERTIES::ISP_EHDR_EXPOSURE_MAX_NUMBER,
                                 min_ehdr, max_ehdr, step, def);
    std::cout << "eHDR min: " << min_ehdr << ", max: " << max_ehdr << ", step: " << step
              << ", default: " << def << std::endl;

    int ehdr_exposures = def;
    // Set the brightness
    std::cout << "Playing video with eHDR exposure = " << ehdr_exposures
              << ", press q in windows to exit..." << std::endl;

    cv::Mat frame;
    uint8_t gain;
    uint32_t exposure;
    while (true) {
        bool ret = cap.read(frame);
        if (ret) {
            VxGetCurrentGain(cam, gain);
            VxGetCurrentExposure(cam, exposure);
            std::string ehdr_label = "eHDR exposures: " + std::to_string(ehdr_exposures);
            std::string label_str = "e: increase exposures, q: exit";
            std::string gain_label = "gain: " + std::to_string(gain);
            std::string exposure_label = "exposure (us): " +std::to_string(exposure);
            draw_label(frame, ehdr_label, cv::Point(20, 60));
            draw_label(frame, label_str, cv::Point(20, 20));
            draw_label(frame, gain_label, cv::Point(20, 80));
            draw_label(frame, exposure_label, cv::Point(20, 100));
            cv::imshow("Adjust eHDR Exposures", frame);

            int key = cv::waitKey(1) & 0xFF;
            if (key == 'e') {
                if (ehdr_exposures < max_ehdr) {
                    ehdr_exposures = ehdr_exposures + step;
                }else{
                    ehdr_exposures = min_ehdr;
                }
                VxSetISPImageProcessing(cam, VX_ISP_IMAGE_PROPERTIES::ISP_EHDR_EXPOSURE_MAX_NUMBER,
                                        ehdr_exposures);
                std::cout << "Set max ehdr exposures: " << ehdr_exposures << std::endl;
            }
            if (key == 'q') {
                break;
            }
        } else {
            std::cout << "Can't receive frame (stream end?). Exiting ..." << std::endl;
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    VxClose(cam);
    return 0;
}