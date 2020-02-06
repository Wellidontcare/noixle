#include "binarizewindow.h"
#include "ui_binarizewindow.h"

BinarizeWindow::BinarizeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinarizeWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    setWindowTitle("Binarize Wizard");
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &BinarizeWindow::update_binarize);
}

BinarizeWindow::~BinarizeWindow()
{
    delete ui;
}

void BinarizeWindow::show_binarize_wizard(JImage image)
{
    if(image.channels() == 3)
        ImageProcessingCollection::convert_color(image, source_image_, cv::COLOR_BGR2GRAY);
    else
        source_image_ = image;
    ui->graphicsView->update_image(image.as_qimage());
    ui->horizontalSlider_2->setValue(127);
    show();
}

void BinarizeWindow::update_binarize(int threshold)
{
    JImage thresholded = source_image_;
    JImage histogram;
    ImageProcessingCollection::histogram_gray_thresh(source_image_, histogram, threshold);
    emit thresh_hist_sig(histogram.as_qimage());
    ImageProcessingCollection::binarize(thresholded, thresholded, threshold);
    ui->graphicsView->update_image(thresholded.as_qimage());
}
