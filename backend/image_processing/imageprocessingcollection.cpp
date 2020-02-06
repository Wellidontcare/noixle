#include "imageprocessingcollection.h"
namespace ImageProcessingCollection {

JImage open_image(std::string file_path)
{
    return JImage(file_path);
}

void invert_image(const JImage& in, JImage& out)
{
    out = in.clone();
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
        histogram_bgr(in, histogram, cumulative);
    }
    else if(in.type() == CV_8UC1){
        histogram_gray(in, histogram, cumulative);
    }

    else throw std::logic_error("Greyvalue histogram is not implemented yet");
}

void histogram_bgr(const JImage &in, JImage &histogram, bool cumulative)
{
    std::vector<cv::Mat> channels;
    cv::split(in, channels);

    std::vector<float> hist_b = calc_hist(channels[0], cumulative);
    std::vector<float> hist_g = calc_hist(channels[1], cumulative);
    std::vector<float> hist_r = calc_hist(channels[2], cumulative);

    int hist_width = 256;
    int hist_height = 250;

    histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0,0,0));

    draw_hist(hist_b, histogram, 0);
    draw_hist(hist_g, histogram, 1);
    draw_hist(hist_r, histogram, 2);
    cv::flip(histogram, histogram, 0);
}


std::vector<float> calc_hist(const JImage &channel, bool cumulative)
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

void draw_hist(std::vector<float> hist, JImage &hist_image, const int channel)
{
    cv::normalize(hist, hist, 0, hist_image.rows, cv::NORM_MINMAX);
    for(int x = 0; x < hist_image.cols; ++x){
        for(int h = 0; h < cvRound(hist[static_cast<size_t>(x)]); ++h){
            (hist_image.at<cv::Vec3b>(h, x))[channel] += 250;
        }
    }
}

void histogram_gray(const JImage &in, JImage &histogram, bool cumulative)
{
    int hist_width = 256;
    int hist_height = 250;

    histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0,0,0));

    std::vector<float> hist = calc_hist(in, cumulative);
    draw_hist(hist, histogram, 0);
    draw_hist(hist, histogram, 1);
    draw_hist(hist, histogram, 2);
    cv::flip(histogram, histogram, 0);
}

void convert_color(const JImage &in, JImage &out, int color)
{
    cv::cvtColor(in, out, color);
}

void equalize(const JImage &in, JImage &out)
{
    out = in.clone();
    int width = out.rows;
    int height = out.cols;
    if(out.channels() == 3){
        std::vector<cv::Mat> channels;
        cv::split(out, channels);
        std::vector<float> hist_b = calc_hist(channels[0], true);
        std::vector<float> hist_g = calc_hist(channels[1], true);
        std::vector<float> hist_r = calc_hist(channels[2], true);
        for(int y = 0; y < height; ++y){
            for(int x = 0; x < width; ++x){
                cv::Vec3b pixel_val = out.at<cv::Vec3b>(x, y);
                pixel_val[0] = static_cast<unsigned char>(hist_b[pixel_val[0]]*(255)/(width*height));
                pixel_val[1] = static_cast<unsigned char>(hist_g[pixel_val[1]]*(255)/(width*height));
                pixel_val[2] = static_cast<unsigned char>(hist_r[pixel_val[2]]*(255)/(width*height));
                out.at<cv::Vec3b>(x, y) = pixel_val;
            }
        }
    }
    if(out.channels() == 1){
        std::vector<float> hist = calc_hist(out, true);
        for(int y = 0; y < height; ++y){
            for(int x = 0; x < width; ++x){
                unsigned char pixel_val = out.at<unsigned char>(x, y);
                pixel_val = static_cast<unsigned char>(hist[pixel_val]*(255)/(width*height));
                out.at<unsigned char>(x, y) = pixel_val;
            }
        }
    }
}

void gamma_correct(const JImage &in, JImage &out, const float gamma_val)
{
    out = in.clone();
    if(out.channels() == 3){
        out.forEach<cv::Vec3b>([gamma_val](cv::Vec3b& pixel, const int* position){pixel[0] = cv::pow((static_cast<double>(pixel[0])/255), 1/gamma_val)*255;
                                                                                  pixel[1] = cv::pow((static_cast<double>(pixel[1])/255), 1/gamma_val)*255;
                                                                                  pixel[2] = cv::pow((static_cast<double>(pixel[2])/255), 1/gamma_val)*255;
                                                                                 });
    }
    if(out.channels() == 1){
        out.forEach<unsigned char>([gamma_val](unsigned char& pixel, const int* position){pixel = cv::pow((static_cast<double>(pixel)/255), gamma_val)*255;});
    }
}

void binarize(const JImage& in, JImage &out, const int threshold)
{
    if(in.channels() != 1){
        throw std::logic_error("Image has to be grayscale!");
    }
    out = in.clone();
    out.forEach<unsigned char>([threshold](unsigned char& pixel, const int* position){
        pixel = pixel >= threshold ? 0 : 255;
    });
}

void histogram_gray_thresh(const JImage &in, JImage &histogram, const int threshold)
{
    histogram_gray(in, histogram);
    int channels = histogram.channels();
    cv::line(histogram, {threshold, 0}, {threshold, 250},{0, 0, 255});
}

}
