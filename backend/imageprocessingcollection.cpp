#include "imageprocessingcollection.h"

ImageProcessingCollection::ImageProcessingCollection() {}

void ImageProcessingCollection::open_image(std::string file_path)
{
 active_image_ = cv::imread(file_path);
}

void ImageProcessingCollection::invert_image()
{
    if(active_image_.empty()){
        throw std::logic_error("No image loaded");
    }
    if(active_image_.channels() == 3){
         using Pixel = cv::Point3_<uint8_t>;
         active_image_.forEach<Pixel>([](Pixel& pixel, const int* position){pixel.x = ~pixel.x;
                                                                          pixel.y = ~pixel.y;
                                                                          pixel.z = ~pixel.z;});
    }
    if(active_image_.channels() == 1){
        active_image_.forEach<unsigned char>([](unsigned char& pixel, const int* position){
            pixel = ~pixel;
        });
    }
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
   if(active_image_.channels() == 3){
       cv::Mat rb_swapped;
       cv::cvtColor(active_image_, rb_swapped, cv::COLOR_BGR2RGB);
       return QImage(static_cast<uchar*>(rb_swapped.data),
                      rb_swapped.cols, rb_swapped.rows,
                      static_cast<int>(rb_swapped.step),
                      QImage::Format::Format_RGB888).copy();
    }
   if(active_image_.channels() == 1){
       return QImage(static_cast<uchar*>(active_image_.data),
                     active_image_.cols, active_image_.rows,
                     static_cast<int>(active_image_.step),
                     QImage::Format_Grayscale8).copy();
   }
   else{
       throw std::logic_error("Invalid image format");
   }
}
