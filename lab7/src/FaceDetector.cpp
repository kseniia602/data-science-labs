#include "FaceDetector.hpp"
#include <iostream>

FaceDetector::FaceDetector() : running(false), hasNewFrame(false) {
    try {
        net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    } catch (...) {
        std::cout << "[ERROR] Net load failed" << std::endl;
    }
}

FaceDetector::~FaceDetector() { stop(); }

void FaceDetector::start() {
    if (!running) {
        running = true;
        worker = std::thread(&FaceDetector::workerLoop, this);
    }
}

void FaceDetector::stop() {
    running = false;
    if (worker.joinable()) worker.join();
}

void FaceDetector::submitFrame(const cv::Mat& frame) {
    // Використовуємо try_lock, щоб основний потік ВЗАГАЛІ ніколи не чекав
    if (mtx.try_lock()) {
        if (!frame.empty()) {
            frame.copyTo(inputFrame);
            hasNewFrame = true;
        }
        mtx.unlock();
    }
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return faces;
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!hasNewFrame || inputFrame.empty()) {
                // Якщо немає кадру - потік спить довше (20мс)
                mtx.unlock(); 
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                mtx.lock();
                continue;
            }
            frame = inputFrame.clone();
            hasNewFrame = false;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        try {
            cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detections = net.forward();

            std::vector<cv::Rect> detected;
            float* data = (float*)detections.data;
            for (int i = 0; i < detections.size[2]; i++) {
                float conf = data[i * 7 + 2];
                if (conf > 0.5) {
                    int x1 = static_cast<int>(data[i * 7 + 3] * frame.cols);
                    int y1 = static_cast<int>(data[i * 7 + 4] * frame.rows);
                    int x2 = static_cast<int>(data[i * 7 + 5] * frame.cols);
                    int y2 = static_cast<int>(data[i * 7 + 6] * frame.rows);
                    detected.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
                }
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                faces = detected;
            }
            
            // ПІСЛЯ кожної детекції даємо системі 30мс на перепочинок
            std::this_thread::sleep_for(std::chrono::milliseconds(30));

        } catch (...) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}