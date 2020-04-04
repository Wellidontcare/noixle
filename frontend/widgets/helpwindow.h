#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
    class HelpWindow;
}

class HelpWindow : public QWidget {
Q_OBJECT
    QStringListModel *help_text_;
public:
    explicit HelpWindow(QWidget *parent = nullptr);

    void add_help_text(const QStringList &text);

    ~HelpWindow();

private:
    Ui::HelpWindow *ui;
};

#endif // HELPWINDOW_H
