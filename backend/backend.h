#ifndef BACKEND_H
#define BACKEND_H
#include "commandparser.h"
#include "imageprocessingcollection.h"
#include <QObject>
#include <unordered_map>
#include <functional>
#include <utility>
#include <QFileDialog>
#include <QStandardPaths>

struct StatusBarInfo{
    QPoint pos;
    std::vector<unsigned char> color_vals;
    QString file_name;
    QString format;
};

class Backend : public QObject
{
    typedef void (Backend::*fn_ptr)();
    Q_OBJECT

    CommandParser parser_;
    std::vector<Command> available_commands_;
    ImageProcessingCollection image_processor_;
    std::unordered_map<std::string, fn_ptr> function_lut_;
    std::vector<Arg> current_args_;
    QString current_file_path;
    QWidget* parent_;
    QStringList command_history_;

public:
    Backend(std::vector<Command> available_commands, QWidget* parent = nullptr);
    void populate_function_lut();
    void update_view();
    void help();
    void exit();
    void open_image();
    void invert();
    void save();
    QString image_format();

public slots:
    void execute_command(QString command);
    void update_status_bar(int x, int y);


signals:
    void help_requested(QString);
    void image_updated(QImage);
    void exit_event();
    void update_status_bar_event(StatusBarInfo);
private:
    void save_to_history(const Command& command);
};

#endif // BACKEND_H
