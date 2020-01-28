#ifndef MOUSETRACKERPIXMAPITEM_H
#define MOUSETRACKERPIXMAPITEM_H
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QObject>

class MouseTrackerPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    MouseTrackerPixmapItem(QPixmap pixmap);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

public slots:

signals:
    void mouse_hovers_pixel_sig(int, int);
};

#endif // MOUSETRACKERPIXMAPITEM_H
