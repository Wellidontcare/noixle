#ifndef IMAGEPROCESSINGCOLLECTION_H
#define IMAGEPROCESSINGCOLLECTION_H

#include <string>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class ImageProcessingCollection
{
cv::Mat active_image_;
public:
    ImageProcessingCollection();
    void open_image(std::string file_path);
    void invert_image();
    void save_image(std::string file_path);
    QImage get();
};

#endif // IMAGEPROCESSINGCOLLECTION_H
