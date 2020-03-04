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

void ZoomEnabledGraphicsView::zoom(bool zoom_in, bool fast_zoom)
{
    double zoom_step;
    if(zoom_in){
        zoom_step = fast_zoom ? 1.5 : 1.1;
    }
    else{
        zoom_step = fast_zoom ? 1/1.5 : 1/1.1;
    }
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->scale(zoom_step, zoom_step);
}

void ZoomEnabledGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(QApplication::keyboardModifiers() == Qt::ControlModifier){
        bool in = true;
        bool out = false;
        bool fast_zoom = true;
        if(event->delta() > 0){
            zoom(in, !fast_zoom);
        }
        else{
            zoom(out, !fast_zoom);
        }
    }
}

void ZoomEnabledGraphicsView::adjust_on_resize()
{
    fitInView(scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
}
