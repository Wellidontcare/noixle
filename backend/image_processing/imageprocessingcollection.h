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
    void histogram_bgr(const JImage& in, JImage& histogram, bool cumulative = false);
    void histogram_gray(const JImage& in, JImage& histogram, bool cumulative = false);
    void histogram_gray_thresh(const JImage& in, JImage& histogram, const int threshold);
    std::vector<float> calc_hist(const JImage& channel, bool cumulative = false);
    void equalize(const JImage& in, JImage& out);
    void draw_hist(std::vector<float> hist, JImage& hist_image, const int channel);
    void gamma_correct(const JImage& in, JImage& out, const float gamma_val);
    void binarize(const JImage& in, JImage& out, const int threshold);
    void rotate(const JImage& in, JImage& out, const int angle);
}


#endif // IMAGEPROCESSINGCOLLECTION_H
