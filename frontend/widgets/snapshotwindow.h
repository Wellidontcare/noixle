#ifndef SNAPSHOTWINDOW_H
#define SNAPSHOTWINDOW_H

#include <QTabWidget>
#include <QHBoxLayout>
#include <vector>
#include "backend/image_processing/jimage.h"
#include "zoomenabledgraphicsview.h"

namespace Ui {
class SnapshotWindow;
}

class SnapshotWindow : public QTabWidget
{
    QHBoxLayout* layout_;
    Q_OBJECT
    std::vector<ZoomEnabledGraphicsView*> views_;

public:
    explicit SnapshotWindow(QWidget *parent = nullptr);
    ~SnapshotWindow();

public slots:
    void add_snapshot(JImage);
signals:

private:
    Ui::SnapshotWindow *ui;
};

#endif // SNAPSHOTWINDOW_H
