#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int id) { cap.open(id); }
bool CameraProvider::isOpened() { return cap.isOpened(); }
cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}