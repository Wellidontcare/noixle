#include "jimage.h"

JImage::JImage(const cv::Mat &mat)
    :cv::Mat(mat)
{}

JImage::JImage(const std::string &file_path)
    : cv::Mat(cv::imread(file_path, cv::IMREAD_COLOR))
{
    file_path_ = file_path;
}

QImage JImage::as_qimage()
{
    if(this->channels() == 3){
        JImage rb_swapped;
        cv::cvtColor(*this, rb_swapped, cv::COLOR_BGR2RGB);
        return QImage(rb_swapped.data,
                      rb_swapped.cols, rb_swapped.rows,
                      static_cast<int>(rb_swapped.step),
                      QImage::Format::Format_RGB888).copy();
    }
    if(this->channels() == 1){
        return QImage(this->data,
                      this->cols, this->rows,
                      static_cast<int>(this->step),
                      QImage::Format_Grayscale8).copy();
    }
    throw std::logic_error("Invalid number of channels");
}

std::string JImage::type_as_string()
{
    switch (type()) {
    case CV_8UC3:
        return "CV_8UC3";
    case CV_8UC1:
        return "CV_8UC1";
    default:
        return "Not implemented yet";
    }
}

unsigned char JImage::r_val_at(int x, int y)
{
    return channel_val_at(x, y, RED);
}

unsigned char JImage::g_val_at(int x, int y)
{
    return channel_val_at(x, y, GREEN);
}

unsigned char JImage::b_val_at(int x, int y)
{
    return channel_val_at(x, y, BLUE);
}

std::string JImage::get_file_path()
{
    return file_path_;
}

cv::Mat JImage::from_qimage(QImage image)
{
    if(image.depth() == 3){
        return cv::Mat(image.height(), image.width(), CV_8UC3, image.bits());
    }
    if(image.depth() == 1){
        return cv::Mat(image.height(), image.width(), CV_8UC1, image.bits());
    }
    throw std::logic_error("Invalid number of channels");
}

unsigned char JImage::channel_val_at(int x, int y, color c)
{
    if(x < 0 || y < 0){
        return 0;
    }
    if(channels() == 1){
        return this->at<unsigned char>(x, y);
    }
    else if(channels() == 3){
    switch (c) {
    case RED:
        return (this->at<cv::Vec3b>(x, y))[2];
    case GREEN:
        return (this->at<cv::Vec3b>(x, y))[1];
    case BLUE:
        return (this->at<cv::Vec3b>(x, y))[0];
    }
}
throw std::logic_error("Wrong number of channels for this operation");
}
