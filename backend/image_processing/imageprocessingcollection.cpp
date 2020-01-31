#include "imageprocessingcollection.h"

JImage ImageProcessingCollection::open_image(std::string file_path)
{
    return JImage(file_path);
}

void ImageProcessingCollection::invert_image(const JImage& in, JImage& out)
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

void ImageProcessingCollection::save_image(const JImage& in, std::string file_path)
{
    if(in.empty()){
        throw std::logic_error("Image is empty");
    }
    cv::imwrite(file_path, in);
}

void ImageProcessingCollection::histogram(const JImage &in, JImage &histogram, bool accumulate)
{
    if(in.type() == CV_8UC3){
        int hist_size = 256;
        float range[] = {0, 256};
        const float* hist_range = {range};
        std::vector<cv::Mat> channels;
        cv::split(in, channels);
        cv::Mat hist_b, hist_g, hist_r;

        cv::calcHist(&channels[0], 1, nullptr, cv::Mat(), hist_b, 1, &hist_size, &hist_range, true, accumulate);
        cv::calcHist(&channels[1], 1, nullptr, cv::Mat(), hist_g, 1, &hist_size, &hist_range, true, accumulate);
        cv::calcHist(&channels[2], 1, nullptr, cv::Mat(), hist_r, 1, &hist_size, &hist_range, true, accumulate);

        int hist_width = 256;
        int hist_height = 250;
        int bin_width = cvRound(static_cast<double>(hist_width/hist_size));

        histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0,0,0));

        cv::normalize(hist_b, hist_b, 0, histogram.rows, cv::NORM_MINMAX);
        cv::normalize(hist_g, hist_g, 0, histogram.rows, cv::NORM_MINMAX);
        cv::normalize(hist_r, hist_r, 0, histogram.rows, cv::NORM_MINMAX);

            for(int x = 0; x < histogram.cols; ++x){
                double h_size = hist_b.at<float>(x);
                int cols = histogram.cols;
                for(int h = 0; h < cvRound(hist_b.at<float>(x)); ++h){
                    histogram.at<cv::Vec3b>(cvRound(h), x) = {250, 0, 0};
                }
                for(int h = 0; h < cvRound(hist_g.at<float>(x)); ++h){
                    histogram.at<cv::Vec3b>(cvRound(h), x) += cv::Vec3b(0, 250, 0);
                }
                for(int h = 0; h < cvRound(hist_r.at<float>(x)); ++h){
                    histogram.at<cv::Vec3b>(cvRound(h), x) += cv::Vec3b(0, 0, 250);
                }
            }

            cv::flip(histogram, histogram, 0);

    }
    else throw std::logic_error("Greyvalue histogram is not implemented yet");
}
