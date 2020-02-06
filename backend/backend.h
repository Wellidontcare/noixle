#ifndef BACKEND_H
#define BACKEND_H

#include <fstream>
#include <functional>
#include <QObject>
#include <QFileDialog>
#include <QStandardPaths>
#include <unordered_map>
#include <utility>

#include "parser/commandparser.h"
#include "parser/filterparser.h"
#include "image_processing/imageprocessingcollection.h"
#include "timer/timer.h"
#include "timer/timethismacro.h"
#include "statusbarinfo.h"

struct BackendData{
    std::vector<Command> available_commands;
    QString current_file_path;
    std::vector<Arg> current_args;
    QStringList command_history;
    int record_start_index;
    int record_stop_index;
    JImage active_image;
    bool meassure_perf = false;
    int snapshot_count = 0;
    int active_snapshot_idx = 0;
};

class Backend : public QObject
{
    typedef void (Backend::*fn_ptr)();
    Q_OBJECT

    CommandParser parser_;
    std::unordered_map<std::string, fn_ptr> function_lut_;
    QWidget* parent_;
    BackendData data_;


public:
    Backend(std::vector<Command> available_commands, QWidget* parent = nullptr);
    void update_view();
    void help();
    void exit();
    void open();
    void load_snapshot();
    void iminvert();
    void save();
    void snapshot();
    void record();
    void history();
    void load_macro();
    void filter();
    void toggle_measure_perf();
    bool meassure_perf();
    void revert();
    void histogram();
    void imcconvert();
    void imequalize();
    void imgammacorrect();

public slots:
    void execute_command(QString command);
    void update_status_bar_on_load();
    void update_status_bar_dynamic(int x, int y);
    void show_performance_info(QString time_taken);
    void set_active_snapshot(int idx);


signals:
    void help_request_sig();
    void image_updated_sig(QImage);
    void exit_sig();
    void update_status_bar_sig(StatusBarInfoStatic);
    void update_status_bar_dynamic_sig(StatusBarInfoDynamic);
    void snapshot_taken_sig(JImage);
    void history_requested_sig(QString);
    void performance_info_requested_sig(QString);
    void histogram_updated_sig(QImage);

private:
    void save_to_history(QString command, QString args);
    void execute_macro(QString file_path);
    void populate_function_lut();
    void backup();
    QString image_format();
    void set_args(const std::vector<Arg>& args);
    Arg construct_arg(int arg);
    Arg construct_arg(double arg);
    Arg construct_arg(std::string arg);
    Arg construct_arg(QString arg);
    bool is_revertable(QString command);
};

#endif // BACKEND_H
