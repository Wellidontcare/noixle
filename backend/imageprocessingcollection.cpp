#include "imageprocessingcollection.h"

ImageProcessingCollection::ImageProcessingCollection()
{

}

void ImageProcessingCollection::open_image(std::string file_path)
{
 active_image_ = new QImage();
 active_image_->load(file_path.c_str());
}

void ImageProcessingCollection::invert_image()
{
    if(!active_image_){
        throw std::logic_error("No image loaded");
    }
    active_image_->invertPixels();
}

void ImageProcessingCollection::save_image(std::string file_path)
{
    if(!active_image_){
        throw std::logic_error("No image loaded");
    }
    active_image_->save(file_path.c_str());
}

QImage ImageProcessingCollection::get()
{
    return *active_image_;
}
