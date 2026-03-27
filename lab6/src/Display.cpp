#include "Display.hpp"

Display::Display(std::string name) : winName(name) {
    cv::namedWindow(winName);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow(winName, frame);
    }
}