#ifndef SNAPSHOTWINDOW_H
#define SNAPSHOTWINDOW_H

#include <QTabWidget>
#include <QHBoxLayout>
#include <vector>
#include "zoomenabledgraphicsview.h"

namespace Ui {
class SnapshotWindow;
}

class SnapshotWindow : public QTabWidget
{
    QHBoxLayout* layout_;
    Q_OBJECT
    std::vector<ZoomEnabledGraphicsView*> views_;
    std::vector<QString> file_paths_;

public:
    explicit SnapshotWindow(QWidget *parent = nullptr);
    ~SnapshotWindow();

public slots:
    void add_snapshot(QImage image, QString file_path);
signals:

private:
    Ui::SnapshotWindow *ui;
};

#endif // SNAPSHOTWINDOW_H
