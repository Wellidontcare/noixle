#include "filterparser.h"

FilterID FilterParser::parse(const QString& filter_string)
{
    if(filter_string.startsWith("[") && filter_string.endsWith("]"))
        return {false, NONE, parse_kernel(filter_string)};
    if(filter_string == "median")
        return {true, MEDIAN, cv::Mat()};
    if(filter_string == "gaussian")
        return {true, GAUSSIAN, cv::Mat()};
    if(filter_string == "binomial")
        return {true, BINOMIAL, cv::Mat()};
    if(filter_string == "sobel")
        return {true, SOBEL, cv::Mat()};
    if(filter_string == "dilate")
        return {true, DILATE, cv::Mat()};
    if(filter_string == "erode")
        return {true, ERODE, cv::Mat()};
    if(filter_string == "laplace")
        return {true, LAPLACE, cv::Mat()};
    return {true, NONE, cv::Mat()};
}

cv::Mat FilterParser::parse_kernel(QString kernel_string)
{
    kernel_string.chop(1);
    kernel_string.remove(0, 1);
    int height =
            static_cast<int>
            (std::count_if(kernel_string.begin(),
                           kernel_string.end(),
                           [](const QChar c){return c == ";";}));
    int width =
            static_cast<int>
            (std::count_if(kernel_string.begin(),
                           kernel_string.end(),
                           [](const QChar c){return c.isDigit();}) / height);
    cv::Mat kernel(width, height, CV_8U);
    auto kernel_ptr = kernel.ptr();
    for(const QChar& c : kernel_string){
        if(c.isSpace() || c == ";")
            continue;
        if(c.isDigit())
        {
            *kernel_ptr = static_cast<unsigned char>(c.digitValue());
            kernel_ptr++;
        }
        else{
            throw std::logic_error("Error in " + std::string(__func__) + "\nEncountered invalid character in filter kernel");
        }
    }
    return kernel;
}
