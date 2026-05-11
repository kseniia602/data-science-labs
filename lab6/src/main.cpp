#include "CameraProvider.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"
#include "Display.hpp"

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        return -1;
    }

    KeyProcessor keys;
    FrameProcessor processor;
    Display display("OpenCV Lab 6");

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        cv::Mat processed = processor.process(frame, keys.currentMode);
        display.show(processed);

        int key = cv::waitKey(30);
        if (key == 27) break; // ESC для виходу
        keys.process(key);
    }

    return 0;
}