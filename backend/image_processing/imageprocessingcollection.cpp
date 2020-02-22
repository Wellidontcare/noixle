#include "imageprocessingcollection.h"
namespace ImageProcessingCollection {

template<typename T>
static T fast_median(std::vector<T> array){
    std::nth_element(array.begin(), array.begin() + array.size()/2, array.end());
    return array[array.size()/2];
}

JImage open_image(std::string file_path)
{
    return JImage(file_path);
}


void invert_image(const JImage& in, JImage& out)
{
    out = in.clone();
    if(out.empty()){
        throw std::logic_error("Error in " + std::string(__func__) + "\nThis is not a valid image");
    }
    if(out.channels() == 3){
        using Pixel = cv::Point3_<unsigned char>;
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
        throw std::logic_error("Error in " + std::string(__func__) + "\nImage is empty");
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

    else throw std::logic_error("Error in " + std::string(__func__) + "\nGrayvalue histogram is not implemented yet!");
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
        throw std::logic_error("Error in " + std::string(__func__) + " \nImage has to be grayscale!");
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

void rotate(const JImage &in, JImage &out, const int angle)
{
    if(angle == 360){
        return;
    }
    if(angle == 180 || angle == -180){
        cv::flip(in, out, 0);
        return;
    }
    if(angle == 90){
        cv::transpose(in, out);
        return;
    }
    if(angle == -90){
        cv::flip(in, out, 0);
        cv::transpose(out, out);
        return;
    }
    int width = in.cols;
    int height = in.rows;

    int c_x = width / 2;
    int c_y = height / 2;
    cv::Mat rotation_matrix = cv::getRotationMatrix2D(cv::Point{c_x, c_y}, angle, 1.0);

    cv::Mat rotated = cv::Mat::zeros(in.rows, in.cols, in.type());
    cv::warpAffine(in, rotated, rotation_matrix, rotated.size());
    out = rotated;
}

void pixelize(const JImage &in, JImage &out, const int pixel_size)
{
    out = in;
    if(out.type() == CV_8UC1){
        pixelize_single_channel(out, pixel_size);
    }
    if(out.type() == CV_8UC3){
        std::vector<cv::Mat> channels;
        cv::split(in, channels);
        for(auto& channel : channels){
            pixelize_single_channel(channel, pixel_size);
        }
        cv::merge(channels, out);
    }
}

void pixelize_single_channel(cv::Mat &image, int pixel_size)
{
    int original_width = image.cols;
    int original_height = image.rows;
    if(image.type() != CV_8UC1){
        throw std::logic_error("Error in " + std::string(__func__) + "This channel type is not supported");
    }
    cv::copyMakeBorder(image, image, pixel_size, pixel_size, pixel_size, pixel_size, cv::BORDER_CONSTANT);
    std::vector<unsigned char> pixel_roi(pixel_size*pixel_size);
    unsigned char median;
    std::vector<unsigned char> roi_vec;
    cv::Mat roi_mat;
    for(int y = 0;  y < original_height; y += pixel_size){
        for(int x = 0; x < original_width; x += pixel_size){
            roi_mat = image(cv::Range(y, y+pixel_size), cv::Range(x, x+pixel_size)).clone();
            roi_mat = roi_mat.reshape(0, 1);
            roi_mat.copyTo(roi_vec);
            median = fast_median(roi_vec);
            for(int i = 0; i < pixel_size; ++i){
                for(int j = 0; j < pixel_size; ++j){
                    image.at<unsigned char>(y+j, x+i) = median;
                }
            }
        }
    }
    image = image(cv::Range(pixel_size, (original_height+pixel_size) - (original_height%pixel_size)), cv::Range(pixel_size, original_width+pixel_size - (original_width%pixel_size)));
}

void integral_image(const JImage &in, JImage &out)
{
    if(in.type() != CV_8UC1){
        throw("Error in " + std::string(__func__) + "\n");
    }
    out = in;
    cv::Mat integral_image = cv::Mat::zeros(in.rows+1, in.cols+1, CV_8UC1);
}

}
