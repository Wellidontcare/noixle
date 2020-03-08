#include "filterparser.h"

SpecialFilter FilterParser::parse(QString filter_string) {
  filter_string = filter_string.toLower();
  if (filter_string == "median")
    return MEDIAN;
  if (filter_string == "gaussian")
    return GAUSSIAN;
  if (filter_string == "bilateral")
    return BILATERAL;
  if (filter_string == "sobel")
    return SOBEL;
  if (filter_string == "dilate")
    return DILATE;
  if (filter_string == "erode")
    return ERODE;
  if (filter_string == "laplace")
    return LAPLACE;
  return NONE;
}

cv::Mat FilterParser::get_custom_kernel() {
  QString
      kernel_string = QInputDialog::getText(nullptr, "Custom Kernel Input", "Sobel Example: [-1 -1 -1; 0 0 0; 1 1 1]");
  return parse_kernel(kernel_string);
}

int FilterParser::get_kernel_size() {
  int size = QInputDialog::getInt(nullptr, "Kernel size", "Please input the kernel size", -1, -1);
  if (!(size % 2)) {
    throw std::logic_error("Error in " + std::string(__func__) + " even kernel size is not supported");
  }
  return size;
}

cv::Size FilterParser::get_kernel_size_xy() {
  int sizex = QInputDialog::getInt(nullptr, "Kernel size x", "Please input the kernel size in x direction", 1, 1);
  int sizey = QInputDialog::getInt(nullptr, "Kernel size y", "Please input the kernel size in y direction", 1, 1);
  if (!(sizex % 2 && sizey % 2)) {
    throw std::logic_error("Error in " + std::string(__func__) + " even kernel size is not supported");
  }
  return {sizex, sizey};
}

double FilterParser::get_sigma() {
  double sigma = QInputDialog::getDouble(nullptr, "Sigma", "Please input the sigma value", 0, 0);
  return sigma;
}

std::pair<int, int> FilterParser::get_xy_order() {
  int x_order = QInputDialog::getInt(nullptr, "x order", "Please input the order of the x derivative", 1, 0);
  int y_order = QInputDialog::getInt(nullptr, "y order", "Please input the kernel size in y deivative", 1, 0);
  return {x_order, y_order};
}
cv::Mat FilterParser::get_strel() {
  QString strel = QInputDialog::getText(nullptr, "Custom Strel input", "Leave empty for default");
  if (strel.isNull()) {
    return cv::Mat();
  }
  return parse_kernel(strel);
}

cv::Mat FilterParser::parse_kernel(QString kernel_string) {
  if (kernel_string.isNull()) {
    return cv::Mat();
  }
  if (!(kernel_string.startsWith("[") && kernel_string.endsWith("]"))) {
    throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid Kernelstring provided");
  }
  kernel_string.chop(1);
  kernel_string.remove(0, 1);
  int height =
      static_cast<int>
      (std::count_if(kernel_string.begin(),
                     kernel_string.end(),
                     [](const QChar c) { return c == ";"; })) + 1;
  int width =
      static_cast<int>
      (std::count_if(kernel_string.begin(),
                     kernel_string.end(),
                     [](const QChar c) { return c.isDigit(); }) / height);
  if (!(width % 2 && height % 2)) {
    throw std::logic_error("Error in " + std::string(__func__) + " even kernel sizes are not supported");
  }
  cv::Mat kernel = cv::Mat::zeros(width, height, CV_64F);
  kernel_string.remove(';');
  std::stringstream stream(kernel_string.toStdString());
  double kernel_content;
  int i = 0;
  while (stream >> kernel_content) {
    kernel.at<double>(i) = kernel_content;
    i++;
    if (i > width * height) {
      throw std::logic_error("Error in " + std::string(__func__) + " provided kernel is corrupt");
    }
  }
  if (i < width * height) {
    throw std::logic_error("Error in " + std::string(__func__) + " provided kernel is corrupt");
  }
  int sum = 0;
  kernel.forEach<double>([&sum](double pixel, const int *pos) { sum += pixel; });
  if (sum != 0) {
    kernel.forEach<double>([&sum](double &pixel, const int *pos) { pixel /= sum; });
  }
  return kernel;
}
