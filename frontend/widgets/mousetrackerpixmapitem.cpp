#include "mousetrackerpixmapitem.h"
MouseTrackerPixmapItem::MouseTrackerPixmapItem(QPixmap pixmap)
    : QObject(), QGraphicsPixmapItem (pixmap)
{
    setAcceptHoverEvents(true);
}

void MouseTrackerPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouse_hovers_pixel(event->pos().x(), event->pos().y());
}
