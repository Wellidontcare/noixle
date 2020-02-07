#include "statusbar.h"

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar (parent)
    ,pos_label_(new QLabel("0 0", this))
    ,rgb_label_(new QLabel("0 0 0", this))
    ,format_label_(new QLabel("None", this))
    ,file_path_label_(new QLabel("No image loaded", this))
    ,dimensions_label_(new QLabel("0x0", this))
    ,spacer_(new QLabel(" | ", this))
{
    addWidget(pos_label_);
    addWidget(spacer_);
    addWidget(rgb_label_);
    addPermanentWidget(format_label_);
    addPermanentWidget(dimensions_label_);
    addPermanentWidget(file_path_label_);
    rgb_label_->setTextFormat(Qt::TextFormat::RichText);
}

StatusBar::~StatusBar()
{}

void StatusBar::update_dynamic(StatusBarInfoDynamic info)
{
    pos_label_->setText("X -> " + QString::number(info.x) + " " + "Y -> " + QString::number(info.y));
    rgb_label_->setText("<b><FONT COLOR= '#FF0000'>" + QString::number(info.r) + "   " + "<FONT COLOR= '#00FF00'>" + QString::number(info.g) + "   " + "<FONT COLOR='#0000FF'>" + QString::number(info.b));
}

void StatusBar::update_on_image_load(StatusBarInfoStatic info)
{
    file_path_label_->setText("Filepath: " + info.file_name);
    format_label_->setText("Format: " + info.format);
    dimensions_label_->setText("Dimensions: " + QString::number(info.dimensions.rwidth()) + "x" + QString::number(info.dimensions.rheight()));
}
