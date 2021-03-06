#include "imageprocessingcollection.h"

namespace ImageProcessingCollection {

    template<typename T>
    static T fast_median(std::vector<T> array) {
        std::nth_element(array.begin(), array.begin() + array.size() / 2, array.end());
        return array[array.size() / 2];
    }

    JImage open_image(const std::string &file_path) {
        return JImage(file_path);
    }

    void invert_image(const JImage &in, JImage &out) {
        out = in.clone();
        if (out.empty()) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nThis is not a valid image");
        }
        if (out.channels() == 3) {
            using Pixel = cv::Point3_<uchar>;
            out.forEach<Pixel>([](Pixel &pixel, const int *position) {
                pixel.x = ~pixel.x;
                pixel.y = ~pixel.y;
                pixel.z = ~pixel.z;
            });
        }
        if (out.channels() == 1) {
            out.forEach<uchar>([](uchar &pixel, const int *position) {
                pixel = ~pixel;
            });
        }
    }

    void save_image(const JImage &in, const std::string &file_path) {
        if (in.empty()) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nImage is empty");
        }
        cv::imwrite(file_path, in);
    }

    void histogram(const JImage &in, JImage &histogram, bool cumulative) {
        if (in.type() == CV_8UC3) {
            histogram_bgr(in, histogram, cumulative);
        } else if (in.type() == CV_8UC1) {
            histogram_gray(in, histogram, cumulative);
        } else
            throw std::logic_error(
                    "Error in " + std::string(__func__) + "\nGrayvalue histogram is not implemented yet!");
    }

    void histogram_bgr(const JImage &in, JImage &histogram, bool cumulative) {
        std::vector<cv::Mat> channels;
        cv::split(in, channels);

        std::vector<float> hist_b = calc_hist(channels[0], cumulative);
        std::vector<float> hist_g = calc_hist(channels[1], cumulative);
        std::vector<float> hist_r = calc_hist(channels[2], cumulative);

        int hist_width = 256;
        int hist_height = 250;

        histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0, 0, 0));

        draw_hist(hist_b, histogram, 0);
        draw_hist(hist_g, histogram, 1);
        draw_hist(hist_r, histogram, 2);
        cv::flip(histogram, histogram, 0);
    }

    std::vector<float> calc_hist(const JImage &channel, bool cumulative) {
        int hist_size = 256;
        float range[] = {0, 256};
        const float *hist_range = {range};
        cv::Mat hist_mat;

        cv::calcHist(&channel, 1, nullptr, cv::Mat(), hist_mat, 1, &hist_size, &hist_range);

        std::vector<float> hist(hist_mat);

        if (cumulative) {
            std::partial_sum(hist.begin(), hist.end(), hist.begin());
        }
        return hist;
    }

    void draw_hist(std::vector<float> hist, JImage &hist_image, const int channel) {
        cv::normalize(hist, hist, 0, hist_image.rows, cv::NORM_MINMAX);
        for (int x = 0; x < hist_image.cols; ++x) {
            for (int h = 0; h < cvRound(hist[static_cast<size_t>(x)]); ++h) {
                (hist_image.at<cv::Vec3b>(h, x))[channel] += 250;
            }
        }
    }

    void histogram_gray(const JImage &in, JImage &histogram, bool cumulative) {
        int hist_width = 256;
        int hist_height = 250;

        histogram = cv::Mat(hist_height, hist_width, CV_8UC3, cv::Scalar(0, 0, 0));

        std::vector<float> hist = calc_hist(in, cumulative);
        draw_hist(hist, histogram, 0);
        draw_hist(hist, histogram, 1);
        draw_hist(hist, histogram, 2);
        cv::flip(histogram, histogram, 0);
    }

    void convert_color(const JImage &in, JImage &out, int color) {
        cv::cvtColor(in, out, color);
    }

    void equalize(const JImage &in, JImage &out) {
        out = in.clone();
        int width = out.rows;
        int height = out.cols;
        if (out.channels() == 3) {
            std::vector<cv::Mat> channels;
            cv::split(out, channels);
            std::vector<float> hist_b = calc_hist(channels[0], true);
            std::vector<float> hist_g = calc_hist(channels[1], true);
            std::vector<float> hist_r = calc_hist(channels[2], true);
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    cv::Vec3b pixel_val = out.at<cv::Vec3b>(x, y);
                    pixel_val[0] = static_cast<uchar>(hist_b[pixel_val[0]] * (255) / (width * height));
                    pixel_val[1] = static_cast<uchar>(hist_g[pixel_val[1]] * (255) / (width * height));
                    pixel_val[2] = static_cast<uchar>(hist_r[pixel_val[2]] * (255) / (width * height));
                    out.at<cv::Vec3b>(x, y) = pixel_val;
                }
            }
        }
        if (out.channels() == 1) {
            std::vector<float> hist = calc_hist(out, true);
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    unsigned char pixel_val = out.at<uchar>(x, y);
                    pixel_val = static_cast<uchar>(hist[pixel_val] * (255) / (width * height));
                    out.at<unsigned char>(x, y) = pixel_val;
                }
            }
        }
    }

    void gamma_correct(const JImage &in, JImage &out, const float gamma_val) {
        out = in.clone();
        if (out.channels() == 3) {
            out.forEach<cv::Vec3b>([gamma_val](cv::Vec3b &pixel, const int *position) {
                pixel[0] = cv::pow((static_cast<double>(pixel[0]) / 255), 1 / gamma_val) * 255;
                pixel[1] = cv::pow((static_cast<double>(pixel[1]) / 255), 1 / gamma_val) * 255;
                pixel[2] = cv::pow((static_cast<double>(pixel[2]) / 255), 1 / gamma_val) * 255;
            });
        }
        if (out.channels() == 1) {
            out.forEach<unsigned char>([gamma_val](unsigned char &pixel, const int *position) {
                pixel = static_cast<unsigned char>(cv::pow(static_cast<double>(pixel) / 255, gamma_val) * 255);
            });
        }
    }

    void binarize(const JImage &in, JImage &out, const int threshold) {
        if (in.channels() != 1) {
            throw std::logic_error("Error in " + std::string(__func__) + " \nImage has to be grayscale!");
        }
        out = in.clone();
        out.forEach<unsigned char>([threshold](unsigned char &pixel, const int *position) {
            pixel = pixel >= threshold ? 0 : 255;
        });
    }

    void histogram_gray_thresh(const JImage &in, JImage &histogram, const int threshold) {
        histogram_gray(in, histogram);
        cv::line(histogram, {threshold, 0}, {threshold, 250}, {0, 0, 255});
    }

    void rotate(const JImage &in, JImage &out, const int angle) {
        if (angle == 360) {
            return;
        }
        if (angle == 180 || angle == -180) {
            cv::flip(in, out, 0);
            return;
        }
        if (angle == 90) {
            cv::transpose(in, out);
            return;
        }
        if (angle == -90) {
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

    void pixelize(const JImage &in, JImage &out, const int pixel_size) {
        out = in;
        if (out.type() == CV_8UC1) {
            pixelize_single_channel(out, pixel_size);
        }
        if (out.type() == CV_8UC3) {
            std::vector<cv::Mat> channels;
            cv::split(in, channels);
            for (auto &channel : channels) {
                pixelize_single_channel(channel, pixel_size);
            }
            cv::merge(channels, out);
        }
    }

    static void pixelize_single_channel(cv::Mat &image, int pixel_size) {
        int original_width = image.cols;
        int original_height = image.rows;
        if (image.type() != CV_8UC1) {
            throw std::logic_error("Error in " + std::string(__func__) + "This channel type is not supported");
        }
        cv::copyMakeBorder(image, image, pixel_size, pixel_size, pixel_size, pixel_size, cv::BORDER_CONSTANT);
        std::vector<uchar> pixel_roi(pixel_size * pixel_size);
        unsigned char median;
        std::vector<uchar> roi_vec;
        cv::Mat roi_mat;
        for (int y = 0; y < original_height; y += pixel_size) {
            for (int x = 0; x < original_width; x += pixel_size) {
                roi_mat = image(cv::Range(y, y + pixel_size), cv::Range(x, x + pixel_size)).clone();
                roi_mat = roi_mat.reshape(0, 1);
                roi_mat.copyTo(roi_vec);
                median = fast_median(roi_vec);
                for (int i = 0; i < pixel_size; ++i) {
                    for (int j = 0; j < pixel_size; ++j) {
                        image.at<uchar>(y + j, x + i) = median;
                    }
                }
            }
        }
        image = image(cv::Range(pixel_size, (original_height + pixel_size) - (original_height % pixel_size)),
                      cv::Range(pixel_size, original_width + pixel_size - (original_width % pixel_size)));
    }

    void integral_image(const JImage &in, JImage &out) {
        if (in.type() != CV_8UC1) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nType not supported");
        }
        in.convertTo(out, CV_32S);
        cv::Mat integral_image = cv::Mat::zeros(in.rows + 1, in.cols + 1, CV_32S);
        //cv::integral(in, integral_image);
        for (int y = 1; y < out.rows; ++y) {
            for (int x = 1; x < out.cols; ++x) {
                int &left_pixel = integral_image.at<int>(y, x - 1);
                int &top_pixel = integral_image.at<int>(y - 1, x);
                int &top_left_pixel = integral_image.at<int>(y - 1, x - 1);
                int &source_pixel = out.at<int>(y, x);
                integral_image.at<int>(y, x) = source_pixel + left_pixel + top_pixel - top_left_pixel;
            }
        }
        cv::normalize(integral_image, integral_image, 0, 255, cv::NORM_MINMAX);
        integral_image.convertTo(integral_image, CV_8UC1);
        out = integral_image;
        out.convertTo(out, CV_8U);
    }

    void shading_correct(const JImage &in, JImage &out) {
        if (in.type() != CV_8UC1) {
            throw std::logic_error("Error in " + std::string(__func__)
                                   + ": invalid image type, image has to be grayscale");
        }
        JImage flat_field = out.clone();
        const int &width = in.cols;
        const int &height = in.rows;
        int ksize = (0.1 * width);
        if (!(ksize & 1))
            ksize++;
        cv::GaussianBlur(flat_field, flat_field, cv::Size(ksize, ksize), 40);
        cv::Scalar mean = cv::mean(flat_field.reshape(1, 0));
        double average = mean[0];
        in.convertTo(out, CV_64F);
        for (int y = 0; y < out.rows; ++y) {
            for (int x = 0; x < out.cols; ++x) {
                if (flat_field.at<uchar>(y, x) == 0) {
                    flat_field.at<uchar>(y, x) = 1;
                }
                out.at<double>(y, x) = ((out.at<double>(y, x)) * average) / (flat_field.at<uchar>(y, x));
            }
        }
        //cv::normalize(out, out, 255, 0, cv::NORM_MINMAX);
        out.convertTo(out, CV_8UC1);
    }

    static void custom_filter_single_channel(const cv::Mat &in, cv::Mat &out, const cv::Mat &custom_kernel) {
        if (in.channels() != 1) {
            throw std::logic_error("Error in " + std::string(__func__) + " invalid number of channels");
        }
        cv::Mat image = in.clone();
        out = cv::Mat::zeros(in.rows, in.cols, CV_8UC1);
        int i_width = in.cols;
        int i_height = in.rows;
        int k_width = custom_kernel.cols;
        int k_height = custom_kernel.rows;
        double sum;
        for (int y = k_height / 2; y < i_height - (k_height / 2); ++y) {
            for (int x = k_width / 2; x < i_width - (k_width / 2); ++x) {
                sum = 0;
                for (int i = -(k_height / 2); i <= (k_height / 2); ++i) {
                    for (int j = -(k_width / 2); j <= (k_width / 2); ++j) {
                        sum += image.at<uchar>(y + i, x + j) *
                               custom_kernel.at<double>(i + (k_height / 2), j + (k_width / 2));
                    }
                }
                out.at<uchar>(y, x) = abs(sum);
            }
        }
    }

    void custom_filter_multi_channel(const JImage in, JImage &out, const JImage &custom_kernel) {
        if (in.channels() == 1) {
            custom_filter_single_channel(in, out, custom_kernel);
        } else {
            std::vector<cv::Mat> channels;
            cv::split(in, channels);
            for (cv::Mat &channel : channels) {
                custom_filter_single_channel(channel, channel, custom_kernel);
            }
            cv::merge(channels, out);
        }
    }

    void median(const JImage &in, JImage &out, int size) {
        cv::medianBlur(in, out, size);
    }

    void gaussian(const JImage &in, JImage &out, cv::Size size, double sigma) {
        cv::GaussianBlur(in, out, size, sigma);
    }

    void bilateral(const JImage &in, JImage &out, double sigma_color, double sigma_space) {
        cv::Mat zeros = cv::Mat::zeros(in.rows, in.cols, CV_8UC3);
        cv::bilateralFilter(in, zeros, -1, sigma_color, sigma_space);
        out = zeros;
    }

    void sobel(const JImage &in, JImage &out, int x_order, int y_order, int size) {
        if (size > 7) {
            throw std::logic_error("Error in " + std::string(__func__) + " kernel size greater 7 not supported");
        }
        cv::Sobel(in, out, -1, x_order, y_order, size);
    }

    void dilate(const JImage &in, JImage &out, cv::Mat strel) {
        cv::dilate(in, out, strel);
    }

    void erode(const JImage &in, JImage &out, cv::Mat strel) {
        cv::erode(in, out, strel);
    }

    void laplace(const JImage &in, JImage &out, int size) {
        if (size > 31) {
            throw std::logic_error("Error in " + std::string(__func__) + " kernel size greater 31 not supported");
        }
        cv::Laplacian(in, out, -1, size);
    }

    JImage make_jimage(cv::MatExpr expr) {
        cv::Mat mat(expr);
        mat.convertTo(mat, CV_8UC1);
        return JImage(mat);
    }

    void resize(const JImage &in, JImage &out, int width, int height) {
        cv::resize(in, out, {width, height});
    }

    void discrete_fourier_transform(JImage in, JImage &out) {
        if (in.channels() > 1) {
            convert_color(in, in, cv::COLOR_BGR2GRAY);
        }
        int opt_rows = cv::getOptimalDFTSize(in.rows);
        int opt_cols = cv::getOptimalDFTSize(in.cols);
        cv::Mat dft_mat;
        cv::copyMakeBorder(in,
                           dft_mat,
                           0,
                           opt_rows - in.rows,
                           0,
                           opt_cols - in.cols,
                           cv::BORDER_CONSTANT,
                           cv::Scalar::all(0));
        cv::Mat plane_0;
        dft_mat.convertTo(plane_0, CV_32F);
        std::vector<cv::Mat> planes;
        cv::Mat complex_plain;
        complex_plain.convertTo(complex_plain, CV_32F);
        cv::Mat plane_1 = cv::Mat::zeros(opt_rows, opt_cols, CV_32F);
        planes.push_back(plane_0);
        planes.push_back(plane_1);
        cv::merge(planes, complex_plain);

        cv::dft(complex_plain, complex_plain);

        cv::split(complex_plain, planes);
        cv::magnitude(planes[0], planes[1], planes[0]);
        cv::Mat mag = planes[0];

        mag += cv::Scalar::all(1);
        cv::log(mag, mag);
        mag = mag(cv::Rect(0, 0, mag.cols & -2, mag.rows & -2));

        int c_x = mag.cols / 2;
        int c_y = mag.rows / 2;

        cv::Rect top_left_rect(0, 0, c_x, c_y);
        cv::Rect top_right_rect(c_x, 0, c_x, c_y);
        cv::Rect bottom_left_rect(0, c_y, c_x, c_y);
        cv::Rect bottom_right_rect(c_x, c_y, c_x, c_y);

        cv::Mat top_left_quadrant = mag(top_left_rect);
        cv::Mat top_right_quadrant = mag(top_right_rect);
        cv::Mat bottom_left_quadrant = mag(bottom_left_rect);
        cv::Mat bottom_right_quadrant = mag(bottom_right_rect);

        cv::Mat tmp;

        top_left_quadrant.copyTo(tmp);
        bottom_right_quadrant.copyTo(top_left_quadrant);
        tmp.copyTo(bottom_right_quadrant);

        bottom_left_quadrant.copyTo(tmp);
        top_right_quadrant.copyTo(bottom_left_quadrant);
        tmp.copyTo(top_right_quadrant);

        cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX);
        mag.convertTo(mag, CV_8UC1);
        out = mag;
    }

    void merge(JImage in1, JImage in2, JImage in3, JImage &out) {
        if ((in1.channels() + in2.channels() + in3.channels()) > 3) {
            throw std::logic_error("Error in: " + std::string(__func__) + " resulting image has to many channels");
        }
        std::vector<cv::MatSize> sizes{in1.size, in2.size, in3.size};
        if (!std::all_of(sizes.begin(), sizes.end(), [in1](cv::MatSize size) { return size == in1.size; })) {
            throw std::logic_error("Error in: " + std::string(__func__) + " channels have to be the same size");
        }
        std::vector<cv::Mat> to_merge{in1, in2, in3};
        cv::merge(to_merge, out);
    }

    void gameoflife(JImage in, JImage &out) {
        out = static_cast<cv::Mat>(cv::Mat::zeros(in.rows, in.cols, CV_8UC1));
        for (int y = 1; y < in.rows - 1; ++y) {
            for (int x = 1; x < in.cols - 1; ++x) {
                int sum = in.at<uchar>(y, x)
                          + in.at<uchar>(y, x - 1)
                          + in.at<uchar>(y, x + 1)
                          + in.at<uchar>(y - 1, x)
                          + in.at<uchar>(y + 1, x)
                          + in.at<uchar>(y - 1, x - 1)
                          + in.at<uchar>(y + 1, x + 1)
                          + in.at<uchar>(y - 1, x + 1)
                          + in.at<uchar>(y + 1, x - 1);
                if (sum == 4 * 255) {
                    continue;
                }
                if (sum == 3 * 255) {
                    out.at<uchar>(y, x) = 255;
                } else {
                    out.at<uchar>(y, x) = 0;
                }
            }
        }
    }

    void otsu(const JImage &in, JImage &out) {
        cv::threshold(in, out, 1, 255, cv::THRESH_OTSU);
    }
}
