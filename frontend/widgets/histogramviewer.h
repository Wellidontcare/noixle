#ifndef HISTOGRAMMVIEWER_H
#define HISTOGRAMMVIEWER_H

#include <QWidget>
#include <QVBoxLayout>

#include "zoomenabledgraphicsview.h"

class HistogramViewer : public QWidget
{
    Q_OBJECT
    ZoomEnabledGraphicsView* view_;
    QVBoxLayout* layout_;


public:
    explicit HistogramViewer(QWidget *parent = nullptr);

public slots:
    void show_histogram(QImage hist);
signals:

};

#endif // HISTOGRAMMVIEWER_H
