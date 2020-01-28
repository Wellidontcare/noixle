#ifndef FILTERPARSER_H
#define FILTERPARSER_H
#include <QString>
#include <opencv2/core.hpp>

enum special_filter{
    NONE = 0,
    MEDIAN,
    GAUSSIAN,
    BINOMIAL,
    SOBEL,
    DILATE,
    ERODE,
    LAPLACE
};

struct FilterID{
    bool is_special;
    special_filter type;
    cv::Mat kernel;
};


class FilterParser
{
public:
    FilterParser() = delete;
    static FilterID parse(QString filter_string);
private:
    static cv::Mat parse_kernel(QString kernel_string);

};

#endif // FILTERPARSER_H
