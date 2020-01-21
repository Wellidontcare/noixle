#include "filterparser.h"

FilterID FilterParser::parse(QString filter_string)
{
    if(filter_string.startsWith("[")){
        return {false, NONE, parse_kernel(filter_string)};
    }
    if(filter_string == "median"){
        return {true, MEDIAN, cv::Mat()};
    }
    if(filter_string == "gaussian"){
        return {true, GAUSSIAN, cv::Mat()};
    }
    if(filter_string == "binomial"){
        return {true, BINOMIAL, cv::Mat()};
    }
    if(filter_string == "sobel"){
        return {true, SOBEL, cv::Mat()};
    }
    if(filter_string == "dilate"){
        return {true, DILATE, cv::Mat()};
    }
    if(filter_string == "erode"){
        return {true, ERODE, cv::Mat()};
    }
}
