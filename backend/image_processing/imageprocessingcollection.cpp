#include "imageprocessingcollection.h"
namespace ImageProcessingCollection {

JImage open_image(std::string file_path)
{
    return JImage(file_path);
}

void invert_image(const JImage& in, JImage& out)
{
    out = in;
    if(out.empty()){
        throw std::logic_error("Not a valid image");
    }
    if(out.channels() == 3){
        using Pixel = cv::Point3_<uint8_t>;
        out.forEach<Pixel>([](Pixel& pixel, const int* position){pixel.x = ~pixel.x;
                                                                 pixel.y = ~pixel.y;
                                                                                                                      pixel.z = ~pixel.z;});
    }
    if(out.channels() == 1){
        out.forEach<unsigned char>([](unsigned char& pixel, const int* position){
            pixel = ~pixel;
        });
    }
}

void save_image(const JImage& in, std::string file_path)
{
    if(in.empty()){
        throw std::logic_error("Image is empty");
    }
    cv::imwrite(file_path, in);
}

void histogram(const JImage &in, JImage &histogram, bool cumulative)
{
    if(in.type() == CV_8UC3){
        histogram_bgr_(in, histogram, cumulative);
     }
    else if(in.type() == CV_8UC1){
        histogram_gray_(in, histogram, cumulative);
    }

        else throw std::logic_error("Greyvalue histogram is not implemented yet");
}

void histogram_bgr_(const JImage &in, JImage &histogram, bool cumulative)
{
    std::vector<cv::Mat> channels;
    cv::split(in, channels);

    std::vector<float> hist_b = calc_hist_(channels[0], cumulative);
    std::vector<float> hist_g = calc_hist_(channels[1], cumulative);
    std::vector<float> hist_r = calc_hist_(channels[2], cumulative);

    int hist_width = 256;
    int hist_height = 250;

    histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0,0,0));

    draw_hist_(hist_b, histogram, 0);
    draw_hist_(hist_g, histogram, 1);
    draw_hist_(hist_r, histogram, 2);
    cv::flip(histogram, histogram, 0);
}


std::vector<float> calc_hist_(const JImage &channel, bool cumulative)
{
    int hist_size = 256;
    float range[] = {0, 256};
    const float* hist_range = {range};
    std::vector<cv::Mat> channels;
    cv::Mat hist_mat;

    cv::calcHist(&channel, 1, nullptr, cv::Mat(), hist_mat, 1, &hist_size, &hist_range);

    std::vector<float> hist(hist_mat);

    if(cumulative){
        std::partial_sum(hist.begin(), hist.end(), hist.begin());
    }
    return hist;
}

void draw_hist_(std::vector<float> hist, JImage &hist_image, const int channel)
{
    cv::normalize(hist, hist, 0, hist_image.rows, cv::NORM_MINMAX);
    for(size_t x = 0; x < static_cast<size_t>(hist_image.cols); ++x){
        for(int h = 0; h < cvRound(hist[x]); ++h){
            (hist_image.at<cv::Vec3b>(h, x))[channel] += 250;
    }
}
}

void histogram_gray_(const JImage &in, JImage &histogram, bool cumulative)
{
    int hist_width = 256;
    int hist_height = 250;

    histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0,0,0));

    std::vector<float> hist = calc_hist_(in, cumulative);
    draw_hist_(hist, histogram, 0);
    draw_hist_(hist, histogram, 1);
    draw_hist_(hist, histogram, 2);
    cv::flip(histogram, histogram, 0);
}

void convert_color(const JImage &in, JImage &out, int color)
{
    cv::cvtColor(in, out, color);
}
}
