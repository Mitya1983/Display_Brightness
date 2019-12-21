#include "image_from_camera.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>


void MT::Image::capture_frame()
{
    cv::VideoCapture _capture;
    _capture.open(0);
    if (!_capture.isOpened()){
        std::string msg = "void Image::capture_image(): Can't init camera - ";
        throw std::runtime_error(msg);
    }
    cv::Mat _initial_frame;
    _capture.read(_initial_frame);
    _capture.release();
    cv::cvtColor(_initial_frame, _frame, cv::COLOR_RGB2GRAY);
}

unsigned char MT::Image::average_grayscale_value()
{
    int sum = 0;
    int devider = 0;
    auto start = _frame.data;
    auto end = _frame.dataend;
    while (start != end){
        sum += *start;
        ++start;
        ++devider;
    }
    return static_cast<unsigned char>(sum / devider);
}

unsigned char MT::Image::most_used_grayscale_value()
{
    std::array<int, 256> colors;
    unsigned char max = -1;
    colors.fill(0);
    auto start = _frame.data;
    auto end = _frame.dataend;
    while (start != end){
        ++colors[*start];
        if (colors[*start] > max){
            max = *start;
        }
        ++start;
    }
    return max;
}
