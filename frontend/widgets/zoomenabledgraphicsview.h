#ifndef ZOOMENABLEDGRAPHICSVIEW_H
#define ZOOMENABLEDGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QApplication>
#include <QKeyEvent>

#include "mousetrackerpixmapitem.h"

class ZoomEnabledGraphicsView : public QGraphicsView {
Q_OBJECT
    QGraphicsScene scene_;
    MouseTrackerPixmapItem *image_item_ = nullptr;
    bool drag_key_released = true;

public:
    ZoomEnabledGraphicsView(QWidget *parent = nullptr);

public slots:

    void update_image(const QImage &image, bool recenter);

    void show_text(const QString &text);

    void emit_update_status_bar_sig(int x, int y);

    void zoom(bool zoom_in, bool fast_zoom);

    void wheelEvent(QWheelEvent *event) override;

signals:

    void update_status_bar_sig(int, int);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // ZOOMENABLEDGRAPHICSVIEW_H
