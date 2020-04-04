#include "mousetrackerpixmapitem.h"

MouseTrackerPixmapItem::MouseTrackerPixmapItem(const QPixmap &pixmap)
        : QObject(), QGraphicsPixmapItem(pixmap) {
    setAcceptHoverEvents(true);
}

void MouseTrackerPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    emit mouse_hovers_pixel_sig(event->pos().x(), event->pos().y());
}
