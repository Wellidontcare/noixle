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
#include "parser/calculation_parser.h"
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
    Backend(const std::vector<Command> &available_commands, QWidget* parent = nullptr);
    void update_view();
    void help();
    void exit();
    void open();
    void load_snapshot();
    void save();
    void snapshot();
    void record();
    void history();
    void load_macro();
    void toggle_measure_perf();
    bool meassure_perf();
    void revert();
    void histogram();
    void iminvert();
    void imfilter();
    void imcconvert();
    void imequalize();
    void imgammacorrect();
    void imbinarize();
    void imrotate();
    void impixelize();
    void imshadingcorrect();
    void imintegral();
    void add();
    void sub();
    void mul();
    void div();
    void sqrt();
    void pow();

public slots:
    void execute_command(const QString &command);
    void update_status_bar_on_load();
    void update_status_bar_dynamic(int x, int y);
    void show_performance_info(const QString &time_taken);
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
    void show_threshold_sig(int);
    void binarize_wizard_sig(JImage);

private:
    void save_to_history(const QString &command, const QString &args);
    void execute_macro(const QString &file_path);
    void populate_function_lut();
    void backup();
    void set_args(const std::vector<Arg>& args);
    JImage get_snapshot_by_index(int idx);
    JImage& get_active_image();
    std::tuple<JImage, double, CalcTypeEnum> get_image_or_scalar_for_calc(CalcType type);
    static Arg construct_arg(int arg);
    static Arg construct_arg(double arg);
    static Arg construct_arg(const std::string &arg);
    static Arg construct_arg(const QString &arg);
    bool is_revertable(const QString &command);
};

#endif // BACKEND_H
