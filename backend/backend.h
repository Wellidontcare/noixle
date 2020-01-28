#ifndef BACKEND_H
#define BACKEND_H

#include <fstream>
#include <functional>
#include <QObject>
#include <QFileDialog>
#include <QStandardPaths>
#include <unordered_map>
#include <utility>

#include "commandparser.h"
#include "filterparser.h"
#include "imageprocessingcollection.h"

struct StatusBarInfo{
    QPoint pos;
    std::vector<unsigned char> color_vals;
    QString file_name;
    QString format;
};

struct BackendData{
    std::vector<Command> available_commands;
    std::vector<Arg> current_args;
    QString current_file_path;
    QStringList command_history;
    int record_start_index;
    int record_stop_index;
};

class Backend : public QObject
{
    typedef void (Backend::*fn_ptr)();
    Q_OBJECT

    CommandParser parser_;
    ImageProcessingCollection image_processor_;
    std::unordered_map<std::string, fn_ptr> function_lut_;
    QWidget* parent_;
    BackendData data_;


public:
    Backend(std::vector<Command> available_commands, QWidget* parent = nullptr);
    void populate_function_lut();
    void update_view();
    void help();
    void exit();
    void open_image();
    void invert();
    void save();
    void snapshot();
    void record();
    void history();
    void load_macro();
    void filter();
    QString image_format();

public slots:
    void execute_command(QString command);
    void update_status_bar(int x, int y);


signals:
    void help_requested(QString);
    void image_updated(QImage);
    void exit_event();
    void update_status_bar_event(StatusBarInfo);
    void snapshot_taken(QImage, QString);
    void history_requested(QString);

private:
    void save_to_history(QString command, QString args);
    void execute_macro(QString file_path);
};

#endif // BACKEND_H
