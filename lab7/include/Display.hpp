#pragma once
#include <opencv2/opencv.hpp>

class Display {
    std::string winName;
public:
    Display(std::string name = "Lab 6 OpenCV");
    void show(const cv::Mat& frame);
};