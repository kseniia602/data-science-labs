#pragma once
#include <opencv2/opencv.hpp>
class CameraProvider {
    cv::VideoCapture cap;
public:
    CameraProvider(int id = 0);
    bool isOpened();
    cv::Mat getFrame();
};