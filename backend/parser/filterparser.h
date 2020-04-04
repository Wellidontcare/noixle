#ifndef FILTERPARSER_H
#define FILTERPARSER_H

#include <QString>
#include <QInputDialog>
#include <opencv2/core.hpp>

#include <QDebug>

enum SpecialFilter {
    NONE = 0,
    MEDIAN,
    GAUSSIAN,
    BILATERAL,
    SOBEL,
    DILATE,
    ERODE,
    LAPLACE
};

class FilterParser {
public:
    FilterParser() = delete;

    static SpecialFilter parse(QString filter_string);

    static cv::Mat get_custom_kernel();

    static cv::Mat get_strel();

    static int get_kernel_size();

    static cv::Mat parse_kernel(QString kernel_string);

    static cv::Size get_kernel_size_xy();

    static double get_sigma();

    static std::pair<int, int> get_xy_order();

private:

};

#endif // FILTERPARSER_H
