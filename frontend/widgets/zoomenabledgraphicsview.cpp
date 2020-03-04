#include "zoomenabledgraphicsview.h"

ZoomEnabledGraphicsView::ZoomEnabledGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
    setScene(&scene_);
    setDragMode(ScrollHandDrag);
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

void ZoomEnabledGraphicsView::zoom(bool zoom_in)
{
    double zoom_step;
    if(zoom_in){
        zoom_step = 1.25;
    }
    else{
        zoom_step = 1/1.25;;
    }
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->scale(zoom_step, zoom_step);
}

void ZoomEnabledGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier){
        bool in = true;
        bool out = false;
        if(event->delta() > 0){
            zoom(in);
        }
        else{
            zoom(out);
        }
    }
}

void ZoomEnabledGraphicsView::adjust_on_resize()
{
    fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}
