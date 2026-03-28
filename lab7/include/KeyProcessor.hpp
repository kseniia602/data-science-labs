#pragma once
#include <opencv2/opencv.hpp>

enum class Mode {
    NORMAL,
    INVERT,
    CANNY,
    BLUR,
    GLITCH,
    FACE  
};

class KeyProcessor {
public:
    Mode currentMode = Mode::NORMAL;
    void process(int key);
};