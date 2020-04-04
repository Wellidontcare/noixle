#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QLabel>
#include <QStatusBar>
#include <QWidget>

#include "backend/statusbarinfo.h"

class StatusBar : public QStatusBar {
Q_OBJECT
    QLabel *pos_label_;
    QLabel *rgb_label_;
    QLabel *format_label_;
    QLabel *file_path_label_;
    QLabel *dimensions_label_;
    QLabel *spacer_;
public:
    StatusBar(QWidget *parent = nullptr);

    ~StatusBar() = default;

public slots:

    void update_dynamic(StatusBarInfoDynamic info);

    void update_on_image_load(StatusBarInfoStatic info);
};

#endif // STATUSBAR_H
