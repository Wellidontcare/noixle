#ifndef IMAGEPROCESSINGCOLLECTION_H
#define IMAGEPROCESSINGCOLLECTION_H

#include <QImage>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "jimage.h"

namespace ImageProcessingCollection{
    JImage open_image(std::string file_path);
    void invert_image(const JImage& in, JImage& out);
    void save_image(const JImage& in, std::string file_path);
    void histogram(const JImage& in, JImage& histogram, bool cumulative = false);
    void convert_color(const JImage& in, JImage& out, int color);
    void histogram_bgr_(const JImage& in, JImage& histogram, bool cumulative = false);
    void histogram_gray_(const JImage& in, JImage& histogram, bool cumulative = false);
    std::vector<float> calc_hist_(const JImage& channel, bool cumulative = false);
    void draw_hist_(std::vector<float> hist, JImage& hist_image, const int channel);
}


#endif // IMAGEPROCESSINGCOLLECTION_H
