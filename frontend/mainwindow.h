#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <backend/backend.h>

//!TODO implement snapshot (opens new window with all snapshots (first time) then adds snapshot to tabview)
//!TODO implement history
//!TODO implement histogramm [mode]
//!TODO implement export [histogramm | image]
//!TODO implement record [file_path]
//!TODO implement resize [w][h]
//!TODO implement scale [ratio][width ratio | height ratio]
//!TODO implement rotate [angle] [deg | rad]
//!TODO implement filter [gauss | binominal | mean | median | max | min] [ksize]
//!TODO implement
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLabel* pos_label_;
    QLabel* rgb_label_;
    QLabel* format_label_;
    QLabel* file_path_label_;
    QStringList options_;
    Backend* backend_;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void execute_command(QString command);
    void update_status_bar(StatusBarInfo info);

private:
    Ui::MainWindow *ui;
    std::vector<Command> add_available_commands();
};
#endif // MAINWINDOW_H
