#ifndef ZOOMENABLEDGRAPHICSVIEW_H
#define ZOOMENABLEDGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
class ZoomEnabledGraphicsView : public QGraphicsView
{

    Q_OBJECT

    QGraphicsScene scene_;
public:
    ZoomEnabledGraphicsView();
    ZoomEnabledGraphicsView(QWidget* parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void update_image(QImage image);
    void show_text(QString text);
signals:
    void update_status_bar(int x, int y);


};

#endif // ZOOMENABLEDGRAPHICSVIEW_H
