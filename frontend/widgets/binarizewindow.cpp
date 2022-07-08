#include "binarizewindow.h"
#include "ui_binarizewindow.h"
#include <qnamespace.h>
#include <sstream>

BinarizeWindow::BinarizeWindow(Backend* backend, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::BinarizeWindow),
	backend(backend){
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    setWindowTitle("Binarize Wizard");
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &BinarizeWindow::update_binarize);
}

BinarizeWindow::~BinarizeWindow() {
    delete ui;
}

void BinarizeWindow::show_binarize_wizard(JImage image) {
    if (image.channels() == 3)
        ImageProcessingCollection::convert_color(image, source_image_, cv::COLOR_BGR2GRAY);
    else
        source_image_ = image;
    ui->graphicsView->update_image(image.as_qimage(), false);
    ui->horizontalSlider_2->setValue(127);
    show();
}

void BinarizeWindow::update_binarize(int threshold) {
    JImage thresholded = source_image_;
    JImage histogram;
    ImageProcessingCollection::histogram_gray_thresh(source_image_, histogram, threshold);
    emit thresh_hist_sig(histogram.as_qimage());
    ImageProcessingCollection::binarize(thresholded, thresholded, threshold);
    ui->graphicsView->update_image(thresholded.as_qimage(), false);
}

void BinarizeWindow::finish_binarization(){
	int val = ui->horizontalSlider_2->value();
	std::stringstream command;
	command << "imbinarize " << val;
	backend->execute_command(QString::fromStdString(command.str()));
	emit hist_close_sig();
	close();
}

void BinarizeWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        finish_binarization();
    } else {
        BinarizeWindow::keyPressEvent(event);
    }
}
