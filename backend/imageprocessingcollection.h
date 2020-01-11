#ifndef IMAGEPROCESSINGCOLLECTION_H
#define IMAGEPROCESSINGCOLLECTION_H

#include <string>
#include <QImage>

class ImageProcessingCollection
{
QImage* active_image_ = nullptr;
public:
    ImageProcessingCollection();
    void open_image(std::string file_path);
    void invert_image();
    void save_image(std::string file_path);
    QImage get();
};

#endif // IMAGEPROCESSINGCOLLECTION_H
