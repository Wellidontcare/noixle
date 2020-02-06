#ifndef BINARIZEWINDOW_H
#define BINARIZEWINDOW_H

#include <QWidget>
#include <backend/image_processing/jimage.h>
#include <backend/image_processing/imageprocessingcollection.h>

namespace Ui {
class BinarizeWindow;
}

class BinarizeWindow : public QWidget
{
    Q_OBJECT
    JImage source_image_;

public:
    explicit BinarizeWindow(QWidget *parent = nullptr);
    ~BinarizeWindow();

public slots:
    void show_binarize_wizard(JImage image);
    void update_binarize(int threshold);

signals:
    void thresh_hist_sig(QImage);

private:
    Ui::BinarizeWindow *ui;
};

#endif // BINARIZEWINDOW_H
