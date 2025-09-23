/*
captureImg.cpp - Demonstrates how to use vizionsdk to control ISP camera brightness

Opens a video window with live camera stream.In the window, keys can be pressed to
increase or decrease the brightness.
u : increase 
d : decrease 
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

    // Get the brightness range
    int min_brightness, max_brightness, step, def;
    VxGetISPImageProcessingRange(cam, VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_BRIGHTNESS, min_brightness,
                                 max_brightness, step, def);
    std::cout << "Brightness min: " << min_brightness << ", max: " << max_brightness
              << ", step: " << step << ", default: " << def << std::endl;

    // Get the brightness
    int brightness, flag;
    VxGetISPImageProcessing(cam, VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_BRIGHTNESS, brightness, flag);
    std::cout << "Intial ISP brightness: " << brightness << ", flag: " << flag << std::endl;

    // Set the brightness
    std::cout << "Playing video with brightness " << brightness << ", press q in windows to exit..."
              << std::endl;

    cv::Mat frame;
    while (true) {
        bool ret = cap.read(frame);
        if (ret) {
            std::string brightness_label = "brightness: " + std::to_string(brightness);
            std::string label_str = "u: increase, d: decrease, q: exit";
            draw_label(frame, brightness_label, cv::Point(20, 20));
            draw_label(frame, label_str, cv::Point(20, 40));
            cv::imshow("Adjust brightness", frame);
            int key = cv::waitKey(1) & 0xFF;
            if (key == 'u') {
                if (brightness < max_brightness) {
                    brightness = brightness + step;
                }
                VxSetISPImageProcessing(cam, VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_BRIGHTNESS,
                                        brightness);
                std::cout << "Set brightness to: " << brightness << std::endl;
            }
            if (key == 'd') {
                if (brightness > min_brightness) {
                    brightness = brightness - step;
                }
                VxSetISPImageProcessing(cam, VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_BRIGHTNESS,
                                        brightness);
                std::cout << "Set brightness to: " << brightness << std::endl;
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