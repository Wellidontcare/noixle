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

void SnapshotWindow::add_snapshot(JImage image)
{
    auto view = new ZoomEnabledGraphicsView(this);
    addTab(view, image.get_file_path().c_str());
    auto scene = new QGraphicsScene;
    auto pixmap = QPixmap::fromImage(image.as_qimage());
    scene->addPixmap(pixmap);
    view->setScene(scene);
    views_.emplace_back(view);
    show();
    setCurrentIndex(count() - 1);
    view->fitInView(scene->itemsBoundingRect(), Qt::AspectRatioMode::KeepAspectRatio);
    setFixedSize(size());
}
