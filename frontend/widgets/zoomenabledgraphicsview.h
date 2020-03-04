#ifndef ZOOMENABLEDGRAPHICSVIEW_H
#define ZOOMENABLEDGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QApplication>

#include "mousetrackerpixmapitem.h"

class ZoomEnabledGraphicsView : public QGraphicsView
{
    Q_OBJECT
    QGraphicsScene scene_;
    MouseTrackerPixmapItem* image_item_ = nullptr;

public:
    ZoomEnabledGraphicsView(QWidget* parent = nullptr);

public slots:
    void adjust_on_resize();
    void update_image(const QImage &image);
    void show_text(const QString &text);
    void emit_update_status_bar_sig(int x, int y);
    void zoom(bool zoom_in);
    void wheelEvent(QWheelEvent *event) override;

signals:
    void update_status_bar_sig(int, int);


};

#endif // ZOOMENABLEDGRAPHICSVIEW_H
