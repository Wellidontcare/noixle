#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QStringListModel>

#include "backend/backend.h"
#include "frontend/widgets/snapshotwindow.h"
#include "frontend/widgets/statusbar.h"
#include "frontend/widgets/histogramviewer.h"
#include "frontend/widgets/helpwindow.h"


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

    HelpWindow* help_window_;
    QStringList options_;
    QStringList help_text_;
    Backend* backend_;
    SnapshotViewer* snapshot_viewer_;
    HistogramViewer* histogram_viewer_;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void execute_command(QString command);
    void show_performance_info(QString performance_info);

private:
    Ui::MainWindow *ui;
    std::vector<Command> add_available_commands();

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
