#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
    cv::dnn::Net net;
    std::thread worker;
    std::mutex mtx;
    std::atomic<bool> running;
    bool hasNewFrame; // Прапорець наявності нового кадру
    cv::Mat inputFrame;
    std::vector<cv::Rect> faces;
    void workerLoop();

public:
    FaceDetector();
    ~FaceDetector();
    void submitFrame(const cv::Mat& frame); // Метод подруги
    std::vector<cv::Rect> getFaces();
    void start();
    void stop();
};