#include "frontend/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,pos_label_(new QLabel("0 0"))
    ,rgb_label_(new QLabel("0 0 0"))
    ,format_label_(new QLabel("None"))
    ,file_path_label_(new QLabel("No image loaded"))
    ,options_()
    ,backend_(new Backend(add_available_commands(), parent))
    ,snapshot_window_(new SnapshotWindow)
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->populate_options(options_);
    ui->statusbar->addWidget(pos_label_);
    ui->statusbar->addWidget(rgb_label_);
    ui->statusbar->addWidget(format_label_);
    ui->statusbar->addWidget(file_path_label_);
    connect(backend_, &Backend::image_updated, ui->graphicsView, &ZoomEnabledGraphicsView::update_image);
    connect(backend_, &Backend::help_requested, ui->graphicsView, &ZoomEnabledGraphicsView::show_text);
    connect(ui->lineEdit, &CommandLineEdit::enter_execute_event, this, &MainWindow::execute_command);
    connect(ui->graphicsView, &ZoomEnabledGraphicsView::update_status_bar, backend_, &Backend::update_status_bar);
    connect(backend_, &Backend::update_status_bar_event, this, &MainWindow::update_status_bar);
    connect(backend_, &Backend::exit_event, qApp, &QApplication::closeAllWindows);
    connect(backend_, &Backend::snapshot_taken, snapshot_window_, &SnapshotWindow::add_snapshot);
    connect(backend_, &Backend::history_requested, ui->graphicsView, &ZoomEnabledGraphicsView::show_text);
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

void MainWindow::update_status_bar(StatusBarInfo info)
{
    QString xy = QString::number(info.pos.x()) + " " + QString::number(info.pos.y());
    QString rgb = "";
    rgb += QString::number(info.color_vals[0]);
    info.color_vals.erase(info.color_vals.begin());
    for(int i : info.color_vals){
        rgb += " ";
        rgb += QString::number(i);
    }
    QString format = info.format;
    QString file_path = info.file_name;
    pos_label_->setText(xy);
    rgb_label_->setText(rgb);
    format_label_->setText(format);
    file_path_label_->setText(file_path);
}

std::vector<Command> MainWindow::add_available_commands()
{
    std::vector<Command> commands = {
        {"open", {}, true, {STRING}, 1, "[file_path (optional)] opens an image"},
        {"invert", {}, true, {}, 0, "inverts the currently opened image"},
        {"help", {}, true, {}, 0, "shows this message"},
        {"exit", {}, true, {}, 0, "exits the programm"},
        {"save", {}, true, {}, 1, "[file_path (optional)] saves the currently opened image"},
        {"snapshot", {}, true, {}, 0, "Saves the image as snapshot and displays it in a new tab-window"},
        {"history", {}, true, {}, 0, "Shows the command history"},
        {"record", {}, false, {STRING}, 1, "[start | stop] starts or stops the command recording"},
        {"load_macro", {}, true, {STRING}, 1, "Loads and plays a recorded macro"},
        {"filter", {}, false, {STRING, INT}, 2, "['median' | 'gaussian' | 'binomial' | 'sobel' | 'dilate' | 'erode' | 'laplace' size"}
    };
    for(Command c : commands){
        options_.append(QString::fromStdString(c.command));
    }
    return commands;
}

