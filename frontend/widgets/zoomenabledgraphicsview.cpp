#include "zoomenabledgraphicsview.h"

ZoomEnabledGraphicsView::ZoomEnabledGraphicsView()
{
    setScene(&scene_);
    setMouseTracking(true);
}

ZoomEnabledGraphicsView::ZoomEnabledGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(&scene_);
    setMouseTracking(true);
}

void ZoomEnabledGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    emit update_status_bar(event->x(), event->y());
}

void ZoomEnabledGraphicsView::update_image(QImage image)
{
 QPixmap pixmap = QPixmap::fromImage(image);
 scene_.addPixmap(pixmap);
 fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}

void ZoomEnabledGraphicsView::show_text(QString text)
{
    scene_.clear();
    scene_.addText(text);
    fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}

void ZoomEnabledGraphicsView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene_.itemsBoundingRect(), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
}
