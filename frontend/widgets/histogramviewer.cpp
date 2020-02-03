#include "histogramviewer.h"

HistogramViewer::HistogramViewer(QWidget *parent)
    : QWidget(parent),
      view_(new ZoomEnabledGraphicsView),
      layout_(new QVBoxLayout)
{
    setWindowFlag(Qt::Window);
    setWindowTitle("Histogram");
    layout_->addWidget(view_);
    setLayout(layout_);
}

void HistogramViewer::show_histogram(QImage hist)
{
    view_->update_image(hist);
    show();
}
