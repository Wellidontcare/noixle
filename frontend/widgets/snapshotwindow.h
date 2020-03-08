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

class SnapshotViewer : public QTabWidget
{
    QHBoxLayout* layout_;
    Q_OBJECT
    std::vector<ZoomEnabledGraphicsView*> views_;

public:
    explicit SnapshotViewer(QWidget *parent = nullptr);
    ~SnapshotViewer();

public slots:
    void add_snapshot(JImage);
    void clear_action();
signals:

private:
    Ui::SnapshotWindow *ui;
};

#endif // SNAPSHOTWINDOW_H
