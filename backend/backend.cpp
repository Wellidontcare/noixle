#include "backend.h"

Backend::Backend(const std::vector<Command> &available_commands, QWidget* parent)
    : parser_(available_commands),
      parent_(parent)
{
    data_.available_commands = available_commands;
    populate_function_lut();
}

void Backend::populate_function_lut()
{
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

}

void Backend::backup()
{
    QString tmp_location = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString backup_img_file_path = tmp_location + "/backup_img.tif";
    ImageProcessingCollection::save_image(data_.active_image, backup_img_file_path.toStdString());
}

void Backend::set_args(const std::vector<Arg> &args)
{
    data_.current_args = args;
}

Arg Backend::construct_arg(int arg)
{
    return {arg, 0, ""};
}

Arg Backend::construct_arg(double arg)
{
    return {0, arg, ""};
}

Arg Backend::construct_arg(const std::string &arg)
{
    return {0, 0, arg};
}

Arg Backend::construct_arg(const QString &arg)
{
    return {0, 0, arg.toStdString()};
}

bool Backend::is_revertable(const QString &command)
{
    return command.startsWith("im");
}

void Backend::update_view()
{
    if(data_.active_image.empty())
        throw std::logic_error("Error in " + std::string(__func__) + "\nNo images loaded");
    emit image_updated_sig(data_.active_image.as_qimage());
}

void Backend::help()
{
    emit help_request_sig();
}

void Backend::exit()
{
    emit exit_sig();
}

void Backend::open()
{
    QString open_file_path;
    if(data_.current_args.empty())
    {
        open_file_path = QFileDialog::getOpenFileName(parent_,
                                                 "Select an image file",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        if(open_file_path.toStdString().empty()){
            return;
        }
        data_.active_image = ImageProcessingCollection::open_image(open_file_path.toStdString());
    }
    else
    {
        open_file_path = QString::fromStdString(data_.current_args[0].string_arg);
        if(QFile::exists(open_file_path)){
           data_.active_image = ImageProcessingCollection::open_image(data_.current_args[0].string_arg);
        }
        else{
            throw std::logic_error("Error in " + std::string(__func__) + "\nImage does not exist!");
        }
    }
    update_view();
    update_status_bar_on_load();
    save_to_history("open", open_file_path);
}

void Backend::load_snapshot()
{
    int indx = data_.active_snapshot_idx;
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if(data_.current_args.empty()){
        QString snaphot_img_path = tmp_file_path + "/Snapshot" + QString::number(indx) + ".tif";
        if(!QFile(snaphot_img_path).exists()){
            throw std::logic_error("Error in " + std::string(__func__) + "\nCan't load snapshot");
        }
        set_args({construct_arg(snaphot_img_path)});
        open();
    }
    else{
        int snapshot_idx = data_.current_args[0].int_arg;
        QString snapshot_img_path = tmp_file_path + "/Snapshot" + QString::number(snapshot_idx) + ".tif";
        set_args({construct_arg(snapshot_img_path)});
        open();
    }

}

void Backend::iminvert()
{
    {
    TIME_THIS
    ImageProcessingCollection::invert_image(data_.active_image, data_.active_image);
    }
    update_view();
    save_to_history("invert", "");
}

void Backend::save()
{
    std::string file_path;
    if(data_.current_args.empty()){
        file_path = QFileDialog::getSaveFileName(parent_,
                                                 "Save image",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)).toStdString();
    }
    else{
        file_path = data_.current_args[0].string_arg;
    }
    if(file_path.empty()){
        return;
    }
    save_to_history("save", file_path.c_str());
    ImageProcessingCollection::save_image(data_.active_image, file_path);
}

void Backend::snapshot()
{
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString snapshot_img_file_path = tmp_file_path + "/Snapshot" + QString::number(data_.snapshot_count) + ".tif";
    ImageProcessingCollection::save_image(data_.active_image, snapshot_img_file_path.toStdString());
    data_.snapshot_count++;
    emit snapshot_taken_sig(data_.active_image);
    save_to_history("snapshot", "");
}

void Backend::record()
{
    if(data_.current_args.empty()){
        throw std::logic_error("Error in " + std::string(__func__) + "\nRecord needs either start or stop arg");
    }
    std::string record_arg = data_.current_args[0].string_arg;
    if(record_arg == "start"){
        data_.record_start_index = data_.command_history.size();
    }
    else if(record_arg == "stop"){
        data_.record_stop_index = data_.command_history.size();
        auto doc_location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString filename = QFileDialog::getSaveFileName(nullptr, "Select location to save macro", doc_location);
        std::fstream save_file(filename.toStdString(), std::ios::app);
        for(int i = data_.record_start_index; i < data_.record_stop_index; ++i){
            save_file << data_.command_history[i].toStdString() << '\n';
        }
    }
    else{
        throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid arguments provided!");
    }
}

void Backend::history()
{
    QString hist_string;
    for(const QString& command : data_.command_history){
        hist_string += command + '\n';
    }
    emit history_requested_sig(hist_string);
}

void Backend::load_macro()
{
    QString file_path;
    if(data_.current_args.empty())
    {
        file_path = QFileDialog::getOpenFileName(parent_,
                                                 "Select the macro file",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        if(file_path.toStdString().empty()){
            return;
        }
    }
    else
    {
        file_path = QString::fromStdString(data_.current_args[0].string_arg);
        if(!QFile::exists(file_path)){
            throw std::logic_error("Error in " + std::string(__func__) + "\nFile does not exist!");
        }
    }
    execute_macro(file_path);
}

void Backend::imfilter()
{   TIME_THIS
    FilterID id = FilterParser::parse(data_.current_args[0].string_arg.c_str());
}

void Backend::toggle_measure_perf()
{
    data_.meassure_perf = !data_.meassure_perf;
}

bool Backend::meassure_perf()
{
    return data_.meassure_perf;
}

void Backend::revert()
{
    QString tmp_file_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString backup_img_file_path = tmp_file_path + "/backup_img.tif";
    if(!QFile(backup_img_file_path).exists())
        throw std::logic_error("Error in " + std::string(__func__) + "\nRevert not possible!");
    set_args({construct_arg(backup_img_file_path)});
    open();
}

void Backend::histogram()
{
    {
    TIME_THIS
    if(data_.current_args.empty()){
        JImage hist;
        ImageProcessingCollection::histogram(data_.active_image, hist);
        emit histogram_updated_sig(hist.as_qimage());
        return;
    }
    if(data_.current_args[0].string_arg == "cumulative"){
        JImage hist;
        ImageProcessingCollection::histogram(data_.active_image, hist, true);
        emit histogram_updated_sig(hist.as_qimage());
        return;
    }
    }
    throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid argument");
}

void Backend::imcconvert()
{
    int color;
    if(data_.current_args[0].string_arg == "color"){
        if(data_.active_image.channels() == 3)
            throw std::logic_error("Error in " + std::string(__func__) + "\nImage is already in color mode");
        color = cv::COLOR_GRAY2BGR;
    }
    else if(data_.current_args[0].string_arg == "gray"){
        if(data_.active_image.channels() == 1){
            throw std::logic_error("Error in " + std::string(__func__) + "\nmImage is already in grayscale mode");
        }
        color = cv::COLOR_BGR2GRAY;
    }
    else{
        throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid conversion code");
    }
    {
    TIME_THIS
    ImageProcessingCollection::convert_color(data_.active_image, data_.active_image, color);
    }
    emit image_updated_sig(data_.active_image.as_qimage());
    update_status_bar_on_load();
}

void Backend::imequalize()
{
    JImage& active_image = data_.active_image;
    {
    TIME_THIS
    ImageProcessingCollection::equalize(active_image, active_image);
    }
    update_view();
}

void Backend::imgammacorrect()
{
    JImage& active_image = data_.active_image;
    {
        TIME_THIS
        ImageProcessingCollection::gamma_correct(active_image, active_image, static_cast<double>(data_.current_args[0].float_arg));
    }
    update_view();
}

void Backend::imbinarize()
{
    if(data_.current_args.empty()){
        emit binarize_wizard_sig(data_.active_image);
        return;
    }
    JImage& active_image = data_.active_image;
    int threshold = data_.current_args[0].int_arg;
    if(active_image.channels() != 1){
        ImageProcessingCollection::convert_color(active_image, active_image, cv::COLOR_BGR2GRAY);
    }
    {
    TIME_THIS
    ImageProcessingCollection::binarize(active_image, active_image, threshold);
    }
    emit show_threshold_sig(threshold);
    update_view();
}

void Backend::imrotate()
{
    int angle = data_.current_args[0].int_arg;
    JImage& image = data_.active_image;
    ImageProcessingCollection::rotate(image, image, angle);
    update_status_bar_on_load();
    update_view();
}

void Backend::impixelize()
{
    JImage& active_image = data_.active_image;
    int& pixel_size = data_.current_args[0].int_arg;
    {
    TIME_THIS
    ImageProcessingCollection::pixelize(active_image, active_image, pixel_size);
    }
    update_status_bar_on_load();
    update_view();
}

void Backend::imshadingcorrect()
{
    JImage& active_image = data_.active_image;
    {
    TIME_THIS
    ImageProcessingCollection::shading_correct(active_image, active_image);
    }
    update_view();
}

void Backend::imintegral()
{
    JImage& active_image = data_.active_image;
    {
      TIME_THIS
    ImageProcessingCollection::integral_image(active_image, active_image);
    }
    update_status_bar_on_load();
    update_view();
}

void Backend::execute_command(const QString &command)
{
    if(is_revertable(command)){
        backup();
    }
    Command exec = parser_.parse(command.toStdString().c_str());
    set_args(exec.args);
    (this->*function_lut_.at(std::string(exec.command)))();
}

void Backend::update_status_bar_on_load()
{
    JImage& image = data_.active_image;
    StatusBarInfoStatic info = {
        image.get_file_path().c_str(),
        image.type_as_string().c_str(),
        QSize(image.rows, image.cols)
    };
    emit update_status_bar_sig(info);
}

void Backend::update_status_bar_dynamic(int x, int y)
{
    JImage& image = data_.active_image;
    int r = image.r_val_at(y, x);
    int g = image.g_val_at(y, x);
    int b = image.b_val_at(y, x);
    emit update_status_bar_dynamic_sig(StatusBarInfoDynamic{y, x, r, g, b});
}

void Backend::show_performance_info(const QString &time_taken)
{
    QString perf_info = "Function took: " + time_taken;
    emit performance_info_requested_sig(perf_info);
}

void Backend::set_active_snapshot(int idx)
{
    data_.active_snapshot_idx = idx;
}

void Backend::save_to_history(const QString &command, const QString &args)
{
    data_.command_history.append(command + " " + args);
}

void Backend::execute_macro(const QString &file_path)
{
    std::fstream macro_file(file_path.toStdString());
    std::string command;
    while(getline(macro_file, command)){
        execute_command(command.c_str());
    }
}
