#ifndef STATUSBAR_H
#define STATUSBAR_H
#include <QStatusBar>
#include <QLabel>
class StatusBar : public QStatusBar
{
    Q_OBJECT
    QLabel* pos_label_;
    QLabel* rgb_label_;
    QLabel* format_label_;
    QLabel* file_path_label_;
public:
    StatusBar();
    void update_dynamic(QString position, QString RGB_or_gray_val);
    void update_on_image_load(QString file_path, QString dimensions, QString format);
};

#endif // STATUSBAR_H
