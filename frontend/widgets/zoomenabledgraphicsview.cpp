#include "zoomenabledgraphicsview.h"

ZoomEnabledGraphicsView::ZoomEnabledGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
    setScene(&scene_);
}

void ZoomEnabledGraphicsView::update_image(const QImage &image)
{
 scene_.clear();
 QPixmap pixmap = QPixmap::fromImage(image);
 image_item_ = new MouseTrackerPixmapItem(pixmap);
 scene_.addItem(image_item_);
 scene_.setSceneRect(image_item_->sceneBoundingRect());
 connect(image_item_, &MouseTrackerPixmapItem::mouse_hovers_pixel_sig, this, &ZoomEnabledGraphicsView::emit_update_status_bar_sig);
 fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}

void ZoomEnabledGraphicsView::show_text(const QString &text)
{
    scene_.clear();
    scene_.addText(text);
    fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}

void ZoomEnabledGraphicsView::emit_update_status_bar_sig(int x, int y)
{
    emit update_status_bar_sig(x, y);
}

void ZoomEnabledGraphicsView::resizeEvent(QResizeEvent *event)
{
    event->accept();
    fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}
