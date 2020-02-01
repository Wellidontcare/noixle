#include "frontend/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,options_()
    ,backend_(new Backend(add_available_commands(), parent))
    ,snapshot_viewer_(new SnapshotViewer(this))
    ,histogram_viewer_(new HistogramViewer(this))
    ,ui(new Ui::MainWindow)
{
    snapshot_viewer_->setWindowFlag(Qt::Window);
    ui->setupUi(this);
    ui->lineEdit->populate_options(options_);
    connect(backend_, &Backend::image_updated_sig, ui->graphicsView, &ZoomEnabledGraphicsView::update_image);
    connect(backend_, &Backend::help_request_sig, ui->graphicsView, &ZoomEnabledGraphicsView::show_text);
    connect(ui->lineEdit, &CommandLineEdit::enter_execute_sig, this, &MainWindow::execute_command);
    connect(ui->graphicsView, &ZoomEnabledGraphicsView::update_status_bar_sig, backend_, &Backend::update_status_bar_dynamic);
    connect(backend_, &Backend::update_status_bar_sig, ui->statusbar, &StatusBar::update_on_image_load);
    connect(backend_, &Backend::update_status_bar_dynamic_sig, ui->statusbar, &StatusBar::update_dynamic);
    connect(backend_, &Backend::exit_sig, qApp, &QApplication::closeAllWindows);
    connect(backend_, &Backend::snapshot_taken_sig, snapshot_viewer_, &SnapshotViewer::add_snapshot);
    connect(backend_, &Backend::history_requested_sig, ui->graphicsView, &ZoomEnabledGraphicsView::show_text);
    connect(backend_, &Backend::performance_info_requested_sig, this, &MainWindow::show_performance_info);
    connect(snapshot_viewer_, &SnapshotViewer::currentChanged, backend_, &Backend::set_active_snapshot);
    connect(backend_, &Backend::histogram_updated_sig, histogram_viewer_, &HistogramViewer::show_histogram);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete backend_;
}

void MainWindow::execute_command(QString command)
{
    try {
        backend_->execute_command(command);
    } catch (std::logic_error e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::show_performance_info(QString performance_info)
{
    if(backend_->meassure_perf()){
        QMessageBox::information(this, "Performance report", performance_info);
    }
}

std::vector<Command> MainWindow::add_available_commands()
{
    std::vector<Command> commands = {
        {"help", {}, true, {}, 0, "shows this message"},
        {"open", {}, true, {STRING}, 1, "[file_path (optional)] opens an image"},
        {"save", {}, true, {}, 1, "[file_path (optional)] saves the currently opened image"},
        {"exit", {}, true, {}, 0, "exits the programm"},
        {"revert", {}, true, {}, 0, "Reverts back to last state"},
        {"toggle_perf_meassurement",{}, true, {}, 0, "Toggles the performance meassurement"},
        {"history", {}, true, {}, 0, "Shows the command history"},
        {"record", {}, false, {STRING}, 1, "[start | stop] starts or stops the command recording"},
        {"load_macro", {}, true, {STRING}, 1, "Loads and plays a recorded macro"},
        {"snapshot", {}, true, {}, 0, "Saves the image as snapshot and displays it in a new tab-window"},
        {"load_snapshot", {}, true, {INT}, 1, "Loads the selected snapshot"},
        {"iminvert", {}, true, {}, 0, "inverts the currently opened image"},
        {"imfilter", {}, false, {STRING, INT}, 2, "['median' | 'gaussian' | 'binomial' | 'sobel' | 'dilate' | 'erode' | 'laplace'] size"},
        {"imcconvert", {}, false, {STRING}, 1, "[gray | color] converts the active image to the specified mode"},
        {"histogram", {}, true, {STRING}, 1, "['cumulative'] Displays a histogram of the currently viewed image"}
    };
    for(Command c : commands){
        options_.append(QString::fromStdString(c.command));
    }
    return commands;
}

