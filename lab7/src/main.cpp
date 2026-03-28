#include "CameraProvider.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include <iostream>

int main() {
    CameraProvider camera(0); // Спробуй 0 або 1
    if (!camera.isOpened()) return -1;

    KeyProcessor keys;
    FrameProcessor processor;
    Display display("Lab 7 - Face Detection");
    FaceDetector faceDetector;

    faceDetector.start();

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // Передаємо кадр детектору (це не блокує відео)
        faceDetector.submitFrame(frame);

        int key = cv::waitKey(50); 
        if (key == 27) break; // ESC
        if (key != -1) keys.process(key);

        // Обробка ефектів
        cv::Mat result = processor.process(frame, keys.currentMode);

        // Малюємо рамки, якщо режим FACE
        if (keys.currentMode == Mode::FACE) {
            std::vector<cv::Rect> currentFaces = faceDetector.getFaces();
            for (auto& r : currentFaces) {
                cv::rectangle(result, r, cv::Scalar(0, 255, 0), 3);
                cv::putText(result, "FACE", cv::Point(r.x, r.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
            }
        }

        display.show(result);
    }

    faceDetector.stop();
    return 0;
}