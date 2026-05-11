#include "FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& input, Mode mode) {
    if (input.empty()) return input;

    cv::Mat output;
    
    if (mode == Mode::NORMAL) {
        output = input.clone();
    } 
    else if (mode == Mode::INVERT) {
        cv::bitwise_not(input, output);
    } 
    else if (mode == Mode::CANNY) {
        cv::Mat gray;
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, output, 100, 200);
        // Перетворюємо назад у BGR, щоб можна було малювати кольоровий текст
        cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    } 
    else if (mode == Mode::BLUR) {
        cv::GaussianBlur(input, output, cv::Size(15, 15), 0);
    } 
    else if (mode == Mode::GLITCH) {
        output = input.clone();
        std::vector<cv::Mat> channels;
        cv::split(input, channels);
        // Простий glitch: міняємо місцями канали
        cv::Mat temp = channels[0];
        channels[0] = channels[2];
        channels[2] = temp;
        cv::merge(channels, output);
    }

    // Додаємо текст (тепер він завжди малюється на 3-канальному зображенні)
    cv::putText(output, "Mode: 1-5, ESC to exit", cv::Point(20, 40), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    
    return output;
}