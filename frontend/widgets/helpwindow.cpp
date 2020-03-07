#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QWidget(parent),
    help_text_(nullptr),
    ui(new Ui::HelpWindow)
{
    setWindowFlags(Qt::Window);
    ui->setupUi(this);
    ui->listView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

void HelpWindow::add_help_text(const QStringList &text)
{
    if(help_text_){
        delete help_text_;
        help_text_ = nullptr;
    }
    help_text_ = new QStringListModel(text, this);
    ui->listView->setModel(help_text_);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
