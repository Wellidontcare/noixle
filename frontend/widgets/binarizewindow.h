#ifndef BINARIZEWINDOW_H
#define BINARIZEWINDOW_H

#include <QWidget>
#include "backend/backend.h"
#include <backend/image_processing/jimage.h>
#include <backend/image_processing/imageprocessingcollection.h>
#include <qwidget.h>


namespace Ui {
    class BinarizeWindow;
}

class BinarizeWindow : public QWidget {
Q_OBJECT
    JImage source_image_;

public:
    BinarizeWindow(Backend* backend, QWidget* parent = nullptr);

    ~BinarizeWindow();

public slots:

    void show_binarize_wizard(JImage image);

    void update_binarize(int threshold);

    void finish_binarization();

signals:

    void thresh_hist_sig(QImage);
    void hist_close_sig();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::BinarizeWindow *ui = nullptr;
    Backend* backend = nullptr;
};

#endif // BINARIZEWINDOW_H
