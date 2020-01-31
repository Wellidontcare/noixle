#ifndef ZOOMENABLEDGRAPHICSVIEW_H
#define ZOOMENABLEDGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "mousetrackerpixmapitem.h"

class ZoomEnabledGraphicsView : public QGraphicsView
{
    Q_OBJECT
    QGraphicsScene scene_;
    MouseTrackerPixmapItem* image_item_ = nullptr;

public:
    ZoomEnabledGraphicsView(QWidget* parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void update_image(QImage image);
    void show_text(QString text);
    void emit_update_status_bar_sig(int x, int y);

signals:
    void update_status_bar_sig(int, int);


};

#endif // ZOOMENABLEDGRAPHICSVIEW_H
