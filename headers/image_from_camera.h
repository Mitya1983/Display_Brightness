#ifndef IMAGE_FROM_CAMERA_H
#define IMAGE_FROM_CAMERA_H

#include <opencv2/core.hpp>

namespace MT{

class Image
{
private:
    cv::Mat _frame;
public:
    //CONSTRUCTORS
    Image() = default;
    Image(const Image&) = delete;
    Image(Image&&) = default;
    //OPERATORS
    Image& operator=(const Image&) = delete ;
    Image& operator=(Image&&) = default;
    //SETTERS

    //GETTERS

    //FUNCTIONS
    void capture_frame();
    [[nodiscard]] uint8_t average_grayscale_value();
    [[nodiscard]] uint8_t most_used_grayscale_value();
    //DESTRUCTOR
    ~Image() = default;
};

}//namespace MT
#endif // IMAGE_FROM_CAMERA_H
