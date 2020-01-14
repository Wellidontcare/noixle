#include "backend.h"

Backend::Backend(std::vector<Command> available_commands, QWidget* parent)
    : parser_(available_commands),
      available_commands_(available_commands),
      parent_(parent)
{
    populate_function_lut();
}

void Backend::populate_function_lut()
{
    function_lut_["open"] = &Backend::open_image;
    function_lut_["invert"] = &Backend::invert;
    function_lut_["help"] = &Backend::help;
    function_lut_["exit"] = &Backend::exit;
    function_lut_["save"] = &Backend::save;
    function_lut_["snapshot"] = &Backend::snapshot;
    function_lut_["record"] = &Backend::record;
    function_lut_["history"] = &Backend::history;
    function_lut_["load_macro"] = &Backend::load_macro;

}

void Backend::update_view()
{
    emit image_updated(image_processor_.get());
}

void Backend::help()
{
    QString help_text = "Available Commands:\n\n";
    for(Command c : available_commands_){
        help_text += c.command.c_str();
        help_text += " -> ";
        help_text += c.help_text.c_str();
        help_text += '\n';
    }
    emit help_requested(help_text);
}

void Backend::exit()
{
    emit exit_event();
}

void Backend::open_image()
{
    QString file_path;
    if(current_args_.empty())
    {
        file_path = QFileDialog::getOpenFileName(parent_,
                                                 "Select an image file",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        if(file_path.toStdString().empty()){
            return;
        }
        image_processor_.open_image(file_path.toStdString());
    }
    else
    {
        file_path = QString::fromStdString(current_args_[0].string_arg);
        if(QFile::exists(file_path)){
            image_processor_.open_image(current_args_[0].string_arg.c_str());
        }
        else{
            throw std::logic_error("Image does not exist!");
        }
    }
    update_view();
    save_to_history("open", file_path);
    current_file_path = file_path;
}

void Backend::invert()
{
    image_processor_.invert_image();
    update_view();
    save_to_history("invert", "");
}

void Backend::save()
{
    std::string file_path;
    if(current_args_.empty()){
        file_path = QFileDialog::getSaveFileName(parent_,
                                                 "Save image",
                                                 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)).toStdString();
    }
    else{
        file_path = current_args_[0].string_arg;
    }
    if(file_path.empty()){
        return;
    }
    save_to_history("save", file_path.c_str());
    image_processor_.save_image(file_path);
}

void Backend::snapshot()
{
    emit snapshot_taken(image_processor_.get(), current_file_path);
    save_to_history("snapshot", "");
}

void Backend::record()
{
    if(current_args_[0].string_arg == "start"){
        record_start_index = command_history_.size();
    }
    else if(current_args_[0].string_arg == "stop"){
        record_stop_index = command_history_.size();
        auto doc_location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString filename = QFileDialog::getSaveFileName(nullptr, "Select location to save macro", doc_location);
        std::fstream save_file(filename.toStdString(), std::ios::app);
        for(int i = record_start_index; i < record_stop_index; ++i){
            save_file << command_history_[i].toStdString() << '\n';
        }
    }
    else{
        throw std::logic_error("Invalid arguments");
    }
}

void Backend::history()
{
    QString hist_string = "";
    for(QString command : command_history_){
        hist_string += command + '\n';
    }
    emit history_requested(hist_string);
}

void Backend::load_macro()
{
    QString file_path;
    if(current_args_.empty())
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
        file_path = QString::fromStdString(current_args_[0].string_arg);
        if(QFile::exists(file_path)){
            image_processor_.open_image(current_args_[0].string_arg.c_str());
        }
        else{
            throw std::logic_error("Image does not exist!");
        }
    }
    execute_macro(file_path);
}

QString Backend::image_format()
{
    QImage::Format format = image_processor_.get().format();
    switch(format){
    case QImage::Format_Mono:
        return "Mono";
    case QImage::Format_BGR30:
        return "BGR30";
    case QImage::Format_RGB16:
        return "RGB16";
    case QImage::Format_RGB30:
        return "RGB30";
    case QImage::Format_RGB32:
        return "RGB32";
    case QImage::Format_ARGB32:
        return "ARGB32";
    case QImage::Format_Alpha8:
        return "Alpha8";
    case QImage::Format_RGB444:
        return "RGB444";
    case QImage::Format_RGB555:
        return "RGB555";
    case QImage::Format_RGB666:
        return "RGB666";
    case QImage::Format_RGB888:
        return "RGB888";
    /*case QImage::Format_RGBA64:
        return "RGBA64";
    case QImage::Format_RGBX64:
        return "RGBX64";*/
    case QImage::Format_Invalid:
        return "Invalid";
    case QImage::Format_RGBA8888:
        return "RGBA8888";
    case QImage::Format_RGBX8888:
        return "RGBX8888";
    case QImage::Format_Grayscale8:
        return "Grayscale8";
    default:
        return "Not implemented yet";
    }
}

void Backend::execute_command(QString command)
{
    Command exec = parser_.parse(command.toStdString().c_str());
    current_args_ = exec.args;
    (this->*function_lut_.at(std::string(exec.command)))();
}

void Backend::update_status_bar(int x, int y)
{
    if(current_file_path == ""){
        return;
    }

    StatusBarInfo info = {
        QPoint(x, y),
        {0, 0, 0},
        current_file_path,
        image_format()
    };
    emit update_status_bar_event(info);
}

void Backend::save_to_history(QString command, QString args)
{
    command_history_.append(command + " " + args);
}

void Backend::execute_macro(QString file_path)
{
    std::fstream macro_file(file_path.toStdString());
    std::string command;
    while(getline(macro_file, command)){
        execute_command(command.c_str());
    }
}
