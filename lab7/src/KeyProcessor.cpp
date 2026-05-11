#include "KeyProcessor.hpp"

void KeyProcessor::process(int key) {
    if (key == '1') currentMode = Mode::NORMAL;
    else if (key == '2') currentMode = Mode::INVERT;
    else if (key == '3') currentMode = Mode::CANNY;
    else if (key == '4') currentMode = Mode::BLUR;
    else if (key == '5') currentMode = Mode::GLITCH;
    if (key == 'f' || key == 'F') {
        currentMode = Mode::FACE;
        std::cout << "MODE CHANGED TO FACE!" << std::endl; // додай це
    }
}