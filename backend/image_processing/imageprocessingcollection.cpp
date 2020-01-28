#include "imageprocessingcollection.h"

JImage ImageProcessingCollection::open_image(std::string file_path)
{
    return JImage(file_path);
}

void ImageProcessingCollection::invert_image(const JImage& in, JImage& out)
{
    out = in;
    if(out.empty()){
        throw std::logic_error("Not a valid image");
    }
    if(out.channels() == 3){
         using Pixel = cv::Point3_<uint8_t>;
         out.forEach<Pixel>([](Pixel& pixel, const int* position){pixel.x = ~pixel.x;
                                                                          pixel.y = ~pixel.y;
                                                                          pixel.z = ~pixel.z;});
    }
    if(out.channels() == 1){
        out.forEach<unsigned char>([](unsigned char& pixel, const int* position){
            pixel = ~pixel;
        });
    }
}

void ImageProcessingCollection::save_image(const JImage& in, std::string file_path)
{
    if(in.empty()){
        throw std::logic_error("Image is empty");
    }
    cv::imwrite(file_path, in);
}
