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
#include "frontend/widgets/binarizewindow.h"
#include "frontend/widgets/commandlineedit.h"

//!TODO implement resize [w][h]
//!TODO implement scale [ratio][width ratio | height ratio]
//!TODO implement filter [gauss | binomial | mean | median | max | min] [ksize]
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool drag_key_released = true;
    HelpWindow* help_window_;
    QStringList options_;
    QStringList help_text_;
    Backend* backend_;
    SnapshotViewer* snapshot_viewer_;
    HistogramViewer* histogram_viewer_;
    BinarizeWindow* binarize_window_;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void execute_command(const QString &command);
    void show_performance_info(const QString &performance_info);

private:
    Ui::MainWindow *ui;
    std::vector<Command> add_available_commands();

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent * event) override;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
