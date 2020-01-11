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
        file_path = QFileDialog::getOpenFileName(parent_, "Select an image file", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
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
    current_file_path = file_path;
}

void Backend::invert()
{
    image_processor_.invert_image();
    update_view();
}

void Backend::save()
{
    std::string file_path;
    if(current_args_.empty()){
        file_path = QFileDialog::getSaveFileName(parent_, "Save image", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)).toStdString();
    }
    else{
        file_path = current_args_[0].string_arg;
    }
    image_processor_.save_image(file_path);
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
