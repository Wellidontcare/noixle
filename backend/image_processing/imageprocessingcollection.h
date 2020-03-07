#ifndef IMAGEPROCESSINGCOLLECTION_H
#define IMAGEPROCESSINGCOLLECTION_H

#include <QImage>
#include <string>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>

#include "jimage.h"

namespace ImageProcessingCollection {
JImage open_image(const std::string &file_path);
void invert_image(const JImage &in, JImage &out);
void save_image(const JImage &in, const std::string &file_path);
void histogram(const JImage &in, JImage &histogram, bool cumulative = false);
void convert_color(const JImage &in, JImage &out, int color);
void histogram_bgr(const JImage &in, JImage &histogram, bool cumulative = false);
void histogram_gray(const JImage &in, JImage &histogram, bool cumulative = false);
void histogram_gray_thresh(const JImage &in, JImage &histogram, const int threshold);
std::vector<float> calc_hist(const JImage &channel, bool cumulative = false);
void equalize(const JImage &in, JImage &out);
void draw_hist(std::vector<float> hist, JImage &hist_image, const int channel);
void gamma_correct(const JImage &in, JImage &out, const float gamma_val);
void binarize(const JImage &in, JImage &out, const int threshold);
void rotate(const JImage &in, JImage &out, const int angle);
void pixelize(const JImage &in, JImage &out, const int pixel_size);
void shading_correct(const JImage &in, JImage &out);
void gaussian(const JImage &in, JImage &out, cv::Size size, double sigma);
void median(const JImage &in, JImage &out, int size);
void bilateral(const JImage &in, JImage &out, double sigma_color, double sigma_space);
void sobel(const JImage &in, JImage &out, int x_order, int y_order, int size);
void dilate(const JImage& in, JImage& out, cv::Mat strel);
void erode(const JImage& in, JImage& out, cv::Mat strel);
void integral_image(const JImage &in, JImage &out);
void laplace(const JImage& in, JImage &out, int size);
void custom_filter_multi_channel(const JImage in, JImage &out, const JImage &custom_kernel);
JImage make_jimage(cv::MatExpr expr);
void resize(const JImage& in, JImage& out, int width, int height);
static void custom_filter_single_channel(const cv::Mat &in, cv::Mat &out, const cv::Mat &custom_kernel);
static void pixelize_single_channel(cv::Mat &in, int pixel_size);
}

#endif // IMAGEPROCESSINGCOLLECTION_H
