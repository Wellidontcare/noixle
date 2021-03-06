﻿#include "backend.h"

Backend::Backend(const std::vector<Command> &available_commands, QWidget *parent)
        : parser_(available_commands),
          parent_(parent) {
    data_.available_commands = available_commands;
    populate_function_lut();
}

void Backend::populate_function_lut() {
    function_lut_["open"] = &Backend::open;
    function_lut_["help"] = &Backend::help;
    function_lut_["exit"] = &Backend::exit;
    function_lut_["save"] = &Backend::save;
    function_lut_["snapshot"] = &Backend::snapshot;
    function_lut_["record"] = &Backend::record;
    function_lut_["history"] = &Backend::history;
    function_lut_["load_macro"] = &Backend::load_macro;
    function_lut_["toggle_perf_measurement"] = &Backend::toggle_measure_perf;
    function_lut_["load_snapshot"] = &Backend::load_snapshot;
    function_lut_["revert"] = &Backend::revert;
    function_lut_["histogram"] = &Backend::histogram;
    function_lut_["imcconvert"] = &Backend::imcconvert;
    function_lut_["imfilter"] = &Backend::imfilter;
    function_lut_["iminvert"] = &Backend::iminvert;
    function_lut_["imequalize"] = &Backend::imequalize;
    function_lut_["imgammacorrect"] = &Backend::imgammacorrect;
    function_lut_["imbinarize"] = &Backend::imbinarize;
    function_lut_["imrotate"] = &Backend::imrotate;
    function_lut_["impixelize"] = &Backend::impixelize;
    function_lut_["imshadingcorrect"] = &Backend::imshadingcorrect;
    function_lut_["imintegral"] = &Backend::imintegral;
    function_lut_["add"] = &Backend::add;
    function_lut_["sub"] = &Backend::sub;
    function_lut_["div"] = &Backend::div;
    function_lut_["mul"] = &Backend::mul;
    function_lut_["imresize"] = &Backend::imresize;
    function_lut_["imdft"] = &Backend::imdft;
    function_lut_["immerge"] = &Backend::merge;
    function_lut_["clear"] = &Backend::clear;
    function_lut_["echo"] = &Backend::echo;
    function_lut_["gol"] = &Backend::gameoflife;
}

void Backend::backup() {
    QString tmp_location = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString backup_img_file_path = tmp_location + "/backup_img.tif";
    ImageProcessingCollection::save_image(data_.active_image, backup_img_file_path.toStdString());
}

void Backend::set_args(const std::vector<Arg> &args) {
    data_.current_args = args;
}

JImage Backend::get_snapshot_by_index(int idx) {
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString snaphot_img_path = tmp_file_path + "/Snapshot" + QString::number(idx) + ".tif";
    if (!QFile(snaphot_img_path).exists()) {
        throw std::logic_error("Error in " + std::string(__func__) + "\nCan't load snapshot");
    }
    return JImage(snaphot_img_path.toStdString());
}

JImage &Backend::get_active_image() {
    if (data_.active_image.empty()) {
        throw std::logic_error("Error in " + std::string(__func__) + " no image loaded");
    }
    return data_.active_image;
}

std::tuple<JImage, double, CalcTypeEnum> Backend::get_image_or_scalar_for_calc(CalcType type) {
    if (type.type == ACTIVE_IMAGE) {
        return {data_.active_image, 0, type.type};
    }
    if (type.type == SNAPSHOT) {
        return {get_snapshot_by_index(type.snapshot_index), 0, type.type};
    }
    if (type.type == SCALAR) {
        return {JImage(), type.scalar, type.type};
    } else {
        throw std::logic_error("Error in " + std::string(__func__) + " invalid type");
    }
}

Arg Backend::construct_arg(int arg) {
    return {arg, 0, ""};
}

Arg Backend::construct_arg(double arg) {
    return {0, arg, ""};
}

Arg Backend::construct_arg(const std::string &arg) {
    return {0, 0, arg};
}

Arg Backend::construct_arg(const QString &arg) {
    return {0, 0, arg.toStdString()};
}

bool Backend::is_revertable(const QString &command) {
    return command.startsWith("im") || command.length() == 3;
}

void Backend::update_view(bool recenter) {
    if (data_.active_image.empty())
        throw std::logic_error("Error in " + std::string(__func__) + "\nNo images loaded");
    emit image_updated_sig(data_.active_image.as_qimage(), recenter);
}

void Backend::help() {
    emit help_request_sig();
}

void Backend::exit() {
    emit exit_sig();
}

void Backend::open() {
    QString open_file_path;
    if (data_.current_args.empty()) {
        open_file_path = QFileDialog::getOpenFileName(parent_,
                                                      "Select an image file",
                                                      QStandardPaths::writableLocation(
                                                              QStandardPaths::PicturesLocation));
        if (open_file_path.toStdString().empty()) {
            return;
        }
        data_.active_image = ImageProcessingCollection::open_image(open_file_path.toStdString());
    } else {
        open_file_path = QString::fromStdString(data_.current_args[0].string_arg);
        if (QFile::exists(open_file_path)) {
            data_.active_image = ImageProcessingCollection::open_image(data_.current_args[0].string_arg);
        } else {
            throw std::logic_error("Error in " + std::string(__func__) + "\nImage does not exist!");
        }
    }
    update_view(true);
    update_status_bar_on_load();
    save_to_history("open", open_file_path);
}

void Backend::load_snapshot() {
    int indx = data_.active_snapshot_idx;
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (data_.current_args.empty()) {
        QString snaphot_img_path = tmp_file_path + "/Snapshot" + QString::number(indx) + ".tif";
        if (!QFile(snaphot_img_path).exists()) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nCan't load snapshot");
        }
        set_args({construct_arg(snaphot_img_path)});
        open();
    } else {
        int snapshot_idx = data_.current_args[0].int_arg;
        QString snapshot_img_path = tmp_file_path + "/Snapshot" + QString::number(snapshot_idx) + ".tif";
        set_args({construct_arg(snapshot_img_path)});
        open();
    }
    save_to_history("load_snapshot", QString::number(indx));
}

void Backend::iminvert() {
    {
        TIME_THIS
        JImage &active_image = get_active_image();
        ImageProcessingCollection::invert_image(active_image, active_image);
    }
    update_view();
    save_to_history("invert", "");
}

void Backend::save() {
    JImage &active_image = get_active_image();
    std::string file_path;
    if (data_.current_args.empty()) {
        file_path = QFileDialog::getSaveFileName(parent_,
                                                 "Save image",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation))
                .toStdString();
    } else {
        file_path = data_.current_args[0].string_arg;
    }
    if (file_path.empty()) {
        return;
    }
    save_to_history("save", file_path.c_str());
    ImageProcessingCollection::save_image(active_image, file_path);
}

void Backend::snapshot() {
    JImage &active_image = get_active_image();
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString snapshot_img_file_path = tmp_file_path + "/Snapshot" + QString::number(data_.snapshot_count) + ".tif";
    ImageProcessingCollection::save_image(active_image, snapshot_img_file_path.toStdString());
    data_.snapshot_count++;
    emit snapshot_taken_sig(active_image);
    save_to_history("snapshot", "");
}

void Backend::record() {
    if (data_.current_args.empty()) {
        throw std::logic_error("Error in " + std::string(__func__) + "\nRecord needs either start or stop arg");
    }
    std::string record_arg = data_.current_args[0].string_arg;
    if (record_arg == "start") {
        data_.record_start_index = data_.command_history.size();
    } else if (record_arg == "stop") {
        data_.record_stop_index = data_.command_history.size();
        auto doc_location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString filename = QFileDialog::getSaveFileName(nullptr, "Select location to save macro", doc_location);
        std::fstream save_file(filename.toStdString(), std::ios::app);
        save_file << data_.command_history[data_.record_start_index].toStdString();
        for (int i = data_.record_start_index + 1; i < data_.record_stop_index; ++i) {
            save_file << '\n' << data_.command_history[i].toStdString();
        }
    } else {
        throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid arguments provided!");
    }
}

void Backend::history() {
    QString hist_string;
    for (const QString &command : data_.command_history) {
        hist_string += command + '\n';
    }
    emit history_requested_sig(hist_string);
}

void Backend::load_macro() {
    QString file_path;
    if (data_.current_args.empty()) {
        file_path = QFileDialog::getOpenFileName(parent_,
                                                 "Select the macro file",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        if (file_path.toStdString().empty()) {
            return;
        }
    } else {
        file_path = QString::fromStdString(data_.current_args[0].string_arg);
        if (!QFile::exists(file_path)) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nFile does not exist!");
        }
    }
    execute_macro(file_path);
}

void Backend::imfilter() {
    TIME_THIS
    JImage &active_image = get_active_image();
    SpecialFilter id = SpecialFilter::NONE;
    if (!data_.current_args.empty()) {
        id = FilterParser::parse(data_.current_args[0].string_arg.c_str());
    }
    switch (id) {
        case NONE: {
            JImage kernel = FilterParser::get_custom_kernel();
            if (kernel.empty()) {
                return;
            }
            {
                TIME_THIS
                ImageProcessingCollection::custom_filter_multi_channel(active_image, active_image, kernel);
            }
            save_to_history("imfilter", "");
            break;
        }
        case MEDIAN: {
            int size = FilterParser::get_kernel_size();
            if (size < 0) {
                size = 3;
            }
            {
                TIME_THIS
                ImageProcessingCollection::median(active_image, active_image, size);
            }
            save_to_history("imfilter", "median");
            break;
        }
        case GAUSSIAN: {
            cv::Size size = FilterParser::get_kernel_size_xy();
            double sigma = FilterParser::get_sigma();
            {
                TIME_THIS
                ImageProcessingCollection::gaussian(active_image, active_image, size, sigma);
            }
            save_to_history("imfilter", "gaussian");
            break;
        }
        case BILATERAL: {
            double sigma_color = FilterParser::get_sigma();
            double sigma_space = FilterParser::get_sigma();
            {
                TIME_THIS
                ImageProcessingCollection::bilateral(active_image, active_image, sigma_color, sigma_space);
            }
            save_to_history("imfilter", "bilateral");
            break;
        }
        case SOBEL: {
            //C++ 17 structured binding
            auto[x_order, y_order] = FilterParser::get_xy_order();
            int size = FilterParser::get_kernel_size();
            {
                TIME_THIS
                ImageProcessingCollection::sobel(active_image, active_image, x_order, y_order, size);
            }
            save_to_history("imfilter", "sobel");
            break;
        }
        case DILATE: {
            cv::Mat strel = FilterParser::get_strel();
            {
                TIME_THIS
                ImageProcessingCollection::dilate(active_image, active_image, strel);
            }
            save_to_history("imfilter", "dilate");
            break;
        }
        case ERODE: {
            cv::Mat strel = FilterParser::get_strel();
            {
                TIME_THIS
                ImageProcessingCollection::erode(active_image, active_image, strel);
            }
            save_to_history("imfilter", "erode");
            break;
        }
        case LAPLACE: {
            int size = FilterParser::get_kernel_size();
            {
                TIME_THIS
                ImageProcessingCollection::laplace(active_image, active_image, size);
            }
            save_to_history("imfilter", "laplace");
            break;
        }
    }
    update_view();
}

void Backend::toggle_measure_perf() {
    data_.meassure_perf = !data_.meassure_perf;
}

bool Backend::meassure_perf() {
    return data_.meassure_perf;
}

void Backend::revert() {
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString backup_img_file_path = tmp_file_path + "/backup_img.tif";
    if (!QFile(backup_img_file_path).exists())
        throw std::logic_error("Error in " + std::string(__func__) + "\nRevert not possible!");
    set_args({construct_arg(backup_img_file_path)});
    open();
}

void Backend::histogram() {
    {
        TIME_THIS
        if (data_.current_args.empty()) {
            JImage hist;
            ImageProcessingCollection::histogram(data_.active_image, hist);
            emit histogram_updated_sig(hist.as_qimage());
            return;
        }
        if (data_.current_args[0].string_arg == "cumulative") {
            JImage hist;
            ImageProcessingCollection::histogram(data_.active_image, hist, true);
            emit histogram_updated_sig(hist.as_qimage());
            save_to_history("histogram", "");
            return;
        }
    }
    throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid argument");
}

void Backend::imcconvert() {
    JImage &active_image = get_active_image();
    int color;
    if (data_.current_args[0].string_arg == "color") {
        if (active_image.channels() == 3)
            throw std::logic_error("Error in " + std::string(__func__) + "\nImage is already in color mode");
        color = cv::COLOR_GRAY2BGR;
    } else if (data_.current_args[0].string_arg == "gray") {
        if (active_image.channels() == 1) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nmImage is already in grayscale mode");
        }
        color = cv::COLOR_BGR2GRAY;
    } else {
        throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid conversion code");
    }
    {
        TIME_THIS
        ImageProcessingCollection::convert_color(active_image, active_image, color);
    }
    save_to_history("imcconvert", data_.current_args[0].string_arg.c_str());
    update_view();
    update_status_bar_on_load();
}

void Backend::imequalize() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::equalize(active_image, active_image);
    }
    save_to_history("imequalize", "");
    update_view();
}

void Backend::imgammacorrect() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::gamma_correct(active_image,
                                                 active_image,
                                                 data_.current_args[0].float_arg);
    }
    save_to_history("imgammacorrect", QString::number(data_.current_args[0].float_arg));
    update_view();
}

void Backend::imbinarize() {
    if (data_.current_args.empty()) {
        save_to_history("imbinarize", "");
        emit binarize_wizard_sig(data_.active_image);
        return;
    }
    JImage &active_image = get_active_image();
    int threshold = data_.current_args[0].int_arg;
    if (active_image.channels() != 1) {
        ImageProcessingCollection::convert_color(active_image, active_image, cv::COLOR_BGR2GRAY);
    }
    if (threshold == -1) {
        TIME_THIS
        ImageProcessingCollection::otsu(active_image, active_image);
    } else {
        TIME_THIS
        ImageProcessingCollection::binarize(active_image, active_image, threshold);
    }
    save_to_history("imbinarize", QString::number(data_.current_args[0].int_arg));
    emit show_threshold_sig(threshold);
    update_view();
}

void Backend::imrotate() {
    JImage &active_image = get_active_image();
    int angle = data_.current_args[0].int_arg;
    ImageProcessingCollection::rotate(active_image, active_image, angle);
    save_to_history("imrotate", QString::number(angle));
    update_status_bar_on_load();
    update_view();
}

void Backend::impixelize() {
    JImage &active_image = get_active_image();
    int &pixel_size = data_.current_args[0].int_arg;
    {
        TIME_THIS
        ImageProcessingCollection::pixelize(active_image, active_image, pixel_size);
    }
    save_to_history("impixelize", QString::number(pixel_size));
    update_status_bar_on_load();
    update_view();
}

void Backend::imshadingcorrect() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::shading_correct(active_image, active_image);
    }
    save_to_history("imshadingcorrect", "");
    update_view();
}

void Backend::imintegral() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::integral_image(active_image, active_image);
    }
    save_to_history("imintegral", "");
    update_status_bar_on_load();
    update_view();
}

void Backend::add() {
    auto type1 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[0].string_arg.c_str()));
    auto type2 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[1].string_arg.c_str()));
    JImage image1 = std::get<0>(type1);
    JImage image2 = std::get<0>(type2);
    image1.convertTo(image1, CV_64F);
    image2.convertTo(image2, CV_64F);
    double scalar1 = std::get<1>(type1);
    double scalar2 = std::get<1>(type2);
    if (std::get<2>(type1) == SCALAR && std::get<2>(type2) == SCALAR) {
        throw std::logic_error("Error in " + std::string(__func__) + " this is not a calculator\n the result is "
                               + std::to_string(scalar1 + scalar2) + " btw");
    }
    JImage &active_image = get_active_image();
    if (std::get<2>(type1) == SCALAR) {
        auto mat = cv::Scalar(scalar1, scalar1, scalar1) + image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else if (std::get<2>(type2) == SCALAR) {
        cv::MatExpr mat = image1 + cv::Scalar(scalar2, scalar2, scalar2);
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else {
        if (image1.size != image2.size) {
            throw std::logic_error("Error in " + std::string(__func__) + " image size has to match");
        }
        cv::MatExpr mat = image1 + image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    }
    save_to_history("add", (data_.current_args[0].string_arg + " " + data_.current_args[1].string_arg).c_str());
    update_status_bar_on_load();
    update_view();
}

void Backend::sub() {
    auto type1 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[0].string_arg.c_str()));
    auto type2 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[1].string_arg.c_str()));
    JImage image1 = std::get<0>(type1);
    JImage image2 = std::get<0>(type2);
    image1.convertTo(image1, CV_64F);
    image2.convertTo(image2, CV_64F);
    double scalar1 = std::get<1>(type1);
    double scalar2 = std::get<1>(type2);
    if (std::get<2>(type1) == SCALAR && std::get<2>(type2) == SCALAR) {
        throw std::logic_error("Error in " + std::string(__func__) + " this is not a calculator\n the result is "
                               + std::to_string(scalar1 - scalar2) + " btw");
    }
    JImage &active_image = get_active_image();
    if (std::get<2>(type1) == SCALAR) {
        auto mat = cv::Scalar(scalar1, scalar1, scalar1) - image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else if (std::get<2>(type2) == SCALAR) {
        auto mat = image1 - cv::Scalar(scalar2, scalar2, scalar2);
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else {
        if (image1.size != image2.size) {
            throw std::logic_error("Error in " + std::string(__func__) + " image size has to match");
        }
        auto mat = image1 - image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    }
    save_to_history("sub", (data_.current_args[0].string_arg + " " + data_.current_args[1].string_arg).c_str());
    update_status_bar_on_load();
    update_view();
}

void Backend::mul() {
    auto type1 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[0].string_arg.c_str()));
    auto type2 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[1].string_arg.c_str()));
    JImage image1 = std::get<0>(type1);
    JImage image2 = std::get<0>(type2);
    image1.convertTo(image1, CV_8UC3);
    image2.convertTo(image2, CV_8UC3);
    double scalar1 = std::get<1>(type1);
    double scalar2 = std::get<1>(type2);
    if (std::get<2>(type1) == SCALAR && std::get<2>(type2) == SCALAR) {
        throw std::logic_error("Error in " + std::string(__func__) + " this is not a calculator\n the result is "
                               + std::to_string(scalar1 * scalar2) + " btw");
    }
    JImage &active_image = get_active_image();
    if (std::get<2>(type1) == SCALAR) {
        auto mat = scalar1 * image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else if (std::get<2>(type2) == SCALAR) {
        cv::MatExpr mat = image1 * scalar2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else {
        if (image1.size != image2.size) {
            throw std::logic_error("Error in " + std::string(__func__) + " image size has to match");
        }
        cv::multiply(image1, image2, active_image);
    }
    save_to_history("mul", (data_.current_args[0].string_arg + " " + data_.current_args[1].string_arg).c_str());
    update_status_bar_on_load();
    update_view();
}

void Backend::div() {
    auto type1 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[0].string_arg.c_str()));
    auto type2 =
            get_image_or_scalar_for_calc(
                    CalculationParser::parse_calc_string(data_.current_args[1].string_arg.c_str()));
    JImage image1 = std::get<0>(type1);
    JImage image2 = std::get<0>(type2);
    image1.convertTo(image1, CV_64F);
    image2.convertTo(image2, CV_64F);
    double scalar1 = std::get<1>(type1);
    double scalar2 = std::get<1>(type2);
    if (std::get<2>(type1) == SCALAR && std::get<2>(type2) == SCALAR) {
        throw std::logic_error("Error in " + std::string(__func__) + " this is not a calculator\n the result is "
                               + std::to_string(scalar1 / scalar2) + " btw");
    }
    JImage &active_image = get_active_image();
    if (std::get<2>(type1) == SCALAR) {
        cv::MatExpr mat = scalar1 / image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else if (std::get<2>(type2) == SCALAR) {
        cv::MatExpr mat = image1 / scalar2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    } else {
        if (image1.size != image2.size) {
            throw std::logic_error("Error in " + std::string(__func__) + " image size has to match");
        }
        cv::MatExpr mat = image1 / image2;
        active_image = ImageProcessingCollection::make_jimage(mat);
    }
    save_to_history("div", (data_.current_args[0].string_arg + " " + data_.current_args[1].string_arg).c_str());
    update_status_bar_on_load();
    update_view();
}

void Backend::imresize() {
    JImage &active_image = get_active_image();
    int width = data_.current_args[0].int_arg;
    int height = data_.current_args[1].int_arg;
    {
        TIME_THIS
        ImageProcessingCollection::resize(active_image, active_image, width, height);
    }
    update_status_bar_on_load();
    update_view();
}

void Backend::imdft() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::discrete_fourier_transform(active_image, active_image);
    }
    save_to_history("imdft", "");
    update_status_bar_on_load();
    update_view();
}

void Backend::merge() {
    JImage &active_image = get_active_image();
    auto type1 = get_image_or_scalar_for_calc(
            CalculationParser::parse_calc_string(data_.current_args[0].string_arg.c_str()));
    auto type2 = get_image_or_scalar_for_calc(
            CalculationParser::parse_calc_string(data_.current_args[1].string_arg.c_str()));
    auto type3 = get_image_or_scalar_for_calc(
            CalculationParser::parse_calc_string(data_.current_args[2].string_arg.c_str()));

    if (std::get<2>(type1) == SCALAR || std::get<2>(type2) == SCALAR || std::get<2>(type3) == SCALAR) {
        throw std::logic_error("Error in: " + std::string(__func__) + ": can't merge with scalars");
    }
    JImage channel1 = std::get<0>(type1);
    JImage channel2 = std::get<0>(type2);
    JImage channel3 = std::get<0>(type3);
    ImageProcessingCollection::convert_color(channel1, channel1, cv::COLOR_BGR2GRAY);
    ImageProcessingCollection::convert_color(channel2, channel2, cv::COLOR_BGR2GRAY);
    ImageProcessingCollection::convert_color(channel3, channel3, cv::COLOR_BGR2GRAY);
    TIME_THIS
    ImageProcessingCollection::merge(channel1, channel2, channel3, active_image);
    save_to_history("div", (data_.current_args[0].string_arg + " " + data_.current_args[1].string_arg + " " +
                            data_.current_args[1].string_arg).c_str());
    update_status_bar_on_load();
    update_view();
}

void Backend::clear() {
    data_.active_image = JImage(cv::Mat(1, 1, CV_8UC1));
    data_.active_snapshot_idx = 0;
    data_.current_args = {};
    data_.current_file_path = "";
    data_.snapshot_count = 0;
    data_.record_stop_index = 0;
    data_.record_start_index = 0;
    update_view();
    update_status_bar_on_load();
    save_to_history("clear", "");
    emit clear_sig();
}

void Backend::echo() {
    QString message = data_.current_args[0].string_arg.c_str();
    QMessageBox::information(parent_, "Info", message);
    save_to_history("echo", message);
}

void Backend::execute_command(const QString &command) {
    if (is_revertable(command)) {
        backup();
    }
    Command exec = parser_.parse(command.toStdString().c_str());
    set_args(exec.args);
    (this->*function_lut_.at(std::string(exec.command)))();
}

void Backend::update_status_bar_on_load() {
    JImage &active_image = get_active_image();
    StatusBarInfoStatic info{
            active_image.get_file_path().c_str(),
            active_image.type_as_string().c_str(),
            QSize(active_image.rows, active_image.cols)
    };
    emit update_status_bar_sig(info);
}

void Backend::update_status_bar_dynamic(int x, int y) {
    JImage &active_image = get_active_image();
    int r = active_image.r_val_at(y, x);
    int g = active_image.g_val_at(y, x);
    int b = active_image.b_val_at(y, x);
    emit update_status_bar_dynamic_sig(StatusBarInfoDynamic{y, x, r, g, b});
}

void Backend::show_performance_info(const QString &time_taken) {
    QString perf_info = "Function took: " + time_taken;
    emit performance_info_requested_sig(perf_info);
}

void Backend::set_active_snapshot(int idx) {
    data_.active_snapshot_idx = idx;
}

void Backend::save_to_history(const QString &command, const QString &args) {
    data_.command_history.append(command + " " + args);
}

void Backend::execute_macro(const QString &file_path) {
    std::fstream macro_file(file_path.toStdString());
    std::string command;
    while (getline(macro_file, command)) {
        execute_command(command.c_str());
    }
}

void Backend::gameoflife() {
    JImage &active_image = get_active_image();
    {
        TIME_THIS
        ImageProcessingCollection::gameoflife(active_image, active_image);
    }
    update_view();
}
void Backend::cleanup_snapshots() {
  QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  for(int i = data_.snapshot_count-1; i >= 0; --i){
    QString snaphot_img_path = tmp_file_path + "/Snapshot" + QString::number(i) + ".tif";
    QFile snapshot(snaphot_img_path);
    snapshot.remove();
  }
}
