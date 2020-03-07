#include "frontend/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,help_window_(new HelpWindow(this))
    ,options_()
    ,backend_(new Backend(add_available_commands(), parent))
    ,snapshot_viewer_(new SnapshotViewer(this))
    ,histogram_viewer_(new HistogramViewer(this))
    ,binarize_window_(new BinarizeWindow(this))
    ,ui(new Ui::MainWindow)
{
    snapshot_viewer_->setWindowFlag(Qt::Window);
    ui->setupUi(this);
    ui->lineEdit->populate_options(options_);
    connect(backend_, &Backend::image_updated_sig, ui->graphicsView, &ZoomEnabledGraphicsView::update_image);
    connect(backend_, &Backend::help_request_sig, this, [this](){help_window_->show();});
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
    connect(backend_, &Backend::binarize_wizard_sig, binarize_window_, &BinarizeWindow::show_binarize_wizard);
    connect(binarize_window_, &BinarizeWindow::thresh_hist_sig, histogram_viewer_, &HistogramViewer::show_histogram);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete backend_;
}

void MainWindow::execute_command(const QString &command)
{
    try {
        backend_->execute_command(command);
    } catch (std::logic_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::show_performance_info(const QString &performance_info)
{
    if(backend_->meassure_perf()){
        QMessageBox::information(this, "Performance report", performance_info);
    }
}

std::vector<Command> MainWindow::add_available_commands()
{
    std::vector<Command> commands = {
        {"help", {}, true, {}, 0, "| shows this message"},
        {"open", {}, true, {STRING}, 1, "[file_path (optional)] | opens an image"},
        {"save", {}, true, {}, 1, "[file_path (optional)] | saves the currently opened image"},
        {"exit", {}, true, {}, 0, "| exits the programm"},
        {"revert", {}, true, {}, 0, " | reverts back to last state"},
        {"toggle_perf_measurement",{}, true, {}, 0, " | toggles the performance measurement"},
        {"history", {}, true, {}, 0, "| shows the command history"},
        {"record", {}, false, {STRING}, 1, "[start | stop] | starts or stops the command recording"},
        {"load_macro", {}, true, {STRING}, 1, " | loads and plays a recorded macro"},
        {"snapshot", {}, true, {}, 0, "| saves the image as snapshot and displays it in a new tab-window"},
        {"load_snapshot", {}, true, {INT}, 1, " | loads the selected snapshot"},
        {"iminvert", {}, true, {}, 0, "| inverts the currently opened image"},
        {"imfilter", {}, true, {STRING}, 1, "['median' | 'gaussian' | 'bilateral' | 'sobel' | 'dilate' | 'erode' | 'laplace'] | applies filter kernel to current image. no argument is custom Kernel"},
        {"imcconvert", {}, false, {STRING}, 1, "[gray | color] | converts the active image to the specified mode"},
        {"histogram", {}, true, {STRING}, 1, "['cumulative'] | displays a histogram of the currently viewed image"},
        {"imequalize", {}, true, {}, 0, "| improves the image contrast by equalizing the histogram"},
        {"imgammacorrect", {}, false, {FLOAT}, 1, "| corrects the gamme by the specified value"},
        {"imbinarize", {}, true, {INT}, 1, "[threshold] | binarizes the active image (if the function is called with no arguments a binarize test wizard is opened)"},
        {"imrotate", {}, false, {INT}, 1, "[angle in deg] | rotates the active image by the given angle to the left"},
        {"impixelize", {}, false, {INT}, 1, "[pixelsize] | pixelizes the active image"},
        {"imshadingcorrect", {}, true, {}, 0, "| corrects the shading on an image"},
        {"imintegral", {}, true, {}, 0, "| calcuates the integralimage of the active image"},
        {"sub", {}, false, {STRING, STRING},2, "[a b] | subtract b from a, image or scalar, i is active image, s marks snaphot images Example: sub s1 s2"},
        {"add", {}, false, {STRING, STRING},2, "[a b] |add b to a, image or scalar, i is active image, s marks snaphot images Example: add i 25"},
        {"mul", {}, false, {STRING, STRING},2, "[a b] | multiply a with b, image or scalar, i is active image, s marks snaphot images Example: mul s1 2"},
        {"div", {}, false, {STRING, STRING},2, "[a b] | divide a by b, image or scalar, i is active image, s marks snaphot images Example: div s2 s1"},
        {"resize", {}, false, {INT, INT}, 2, " [width, heigh] | resize image to give arguments"}
    };
    for(const Command& c : commands){
        options_.append(QString::fromStdString(c.command));
        help_text_.append(QString::fromStdString(c.command) + " " + QString::fromStdString(c.help_text));
    }
    help_text_.append("Use Tab to autocomplete!");
    help_text_.append("You can zoom with scrolling in and out while pressing Ctrl or by using the + and - keys");
    help_window_->add_help_text(help_text_);
    return commands;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Alt){
        drag_key_released = true;
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }
    else{
        QMainWindow::keyReleaseEvent(event);
    }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1){
        backend_->help();
    }
    if(event->key() == Qt::Key_Plus){
        ui->graphicsView->zoom(true, true);
    }
    if(event->key() == Qt::Key_Minus){
        ui->graphicsView->zoom(false, true);
    }
    if(event->key() == Qt::Key_Alt){
        if(drag_key_released == true){
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            drag_key_released = false;
        }
    }
    else{
        QMainWindow::keyPressEvent(event);
    }
}

