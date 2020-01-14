#include "snapshotwindow.h"
#include "ui_snapshotwindow.h"

SnapshotWindow::SnapshotWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SnapshotWindow)
{
    ui->setupUi(this);
    setTabsClosable(false);
}

SnapshotWindow::~SnapshotWindow()
{
    delete ui;
    for(auto view : views_){
        delete view;
    }
}

void SnapshotWindow::add_snapshot(QImage image, QString file_path)
{
    auto view = new ZoomEnabledGraphicsView(this);
    addTab(view, file_path);
    auto scene = new QGraphicsScene;
    auto pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
    view->setScene(scene);
    views_.emplace_back(view);
    file_paths_.emplace_back(file_path);
    show();
    setCurrentIndex(count() - 1);
    view->fitInView(scene->itemsBoundingRect(), Qt::AspectRatioMode::KeepAspectRatio);
    setFixedSize(size());
}
