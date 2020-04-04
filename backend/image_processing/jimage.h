#ifndef JIMAGE_H
#define JIMAGE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>

enum color {
    RED,
    GREEN,
    BLUE
};

class JImage : public cv::Mat {
    std::string file_path_;

public:
    using cv::Mat::Mat;

    JImage(const cv::Mat &mat);

    JImage(const std::string &file_path);

    QImage as_qimage();

    std::string type_as_string();

    unsigned char r_val_at(int x, int y);

    unsigned char g_val_at(int x, int y);

    unsigned char b_val_at(int x, int y);

    std::string get_file_path();

private:
    static cv::Mat from_qimage(QImage image);

    unsigned char channel_val_at(int x, int y, color c);

};

#endif // JIMAGE_H
