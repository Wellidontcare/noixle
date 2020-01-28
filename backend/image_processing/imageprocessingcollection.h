#ifndef IMAGEPROCESSINGCOLLECTION_H
#define IMAGEPROCESSINGCOLLECTION_H

#include <QImage>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "jimage.h"

namespace ImageProcessingCollection{
    JImage open_image(std::string file_path);
    void invert_image(const JImage& in, JImage& out);
    void save_image(const JImage& in, std::string file_path);
}

#endif // IMAGEPROCESSINGCOLLECTION_H
