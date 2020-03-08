#include "snapshotwindow.h"
#include "ui_snapshotwindow.h"

SnapshotViewer::SnapshotViewer(QWidget *parent) :
    QTabWidget(parent),
    layout_(nullptr),
    ui(new Ui::SnapshotWindow)
{
    ui->setupUi(this);
    setTabsClosable(false);
    setWindowFlag(Qt::Window);
}


SnapshotViewer::~SnapshotViewer()
{
    delete ui;
}

void SnapshotViewer::add_snapshot(JImage image)
{
    auto view = new ZoomEnabledGraphicsView(this);
    addTab(view, QString::number(views_.size()));
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

void SnapshotViewer::clear_action()
{
    for(auto& view : views_){
        if(view){
            delete view;
        }
    }
    views_.clear();
    clear();
}
