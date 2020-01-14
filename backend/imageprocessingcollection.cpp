#include "imageprocessingcollection.h"

ImageProcessingCollection::ImageProcessingCollection() {}

void ImageProcessingCollection::open_image(std::string file_path)
{
 active_image_ = cv::imread(file_path);
}

void ImageProcessingCollection::invert_image()
{
    using Pixel = cv::Point3_<uint8_t>;
    if(active_image_.empty()){
        throw std::logic_error("No image loaded");
    }
    if(active_image_.channels() == 3)
        active_image_.forEach<Pixel>([](Pixel& pixel, const int* position){pixel.x = ~pixel.x;
                                                                          pixel.y = ~pixel.y;
                                                                          pixel.z = ~pixel.z;});
}

void ImageProcessingCollection::save_image(std::string file_path)
{
    if(active_image_.empty()){
        throw std::logic_error("No image loaded");
    }
    cv::imwrite(file_path.c_str(), active_image_);
}

QImage ImageProcessingCollection::get()
{
   if(active_image_.empty()){
        throw std::logic_error("No image loaded");
   }
   cv::Mat rb_swapped;
   cv::cvtColor(active_image_, rb_swapped, cv::COLOR_BGR2RGB);
   return QImage(static_cast<uchar*>(rb_swapped.data),
                  rb_swapped.cols, rb_swapped.rows,
                  rb_swapped.step,
                  QImage::Format::Format_RGB888).copy();
}
