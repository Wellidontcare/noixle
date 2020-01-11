#ifndef COMMANDLINEEDIT_H
#define COMMANDLINEEDIT_H

#include <QLineEdit>
#include <QAbstractItemView>
#include <QCompleter>
#include <QStringList>
#include <QKeyEvent>
#include <QDebug>
#include <vector>

class CommandLineEdit : public QLineEdit{
    Q_OBJECT

    QStringList available_options;
    QCompleter* completer;

public:
    CommandLineEdit(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent *) override;
    bool event(QEvent *) override;
    void populate_options(QStringList list);

public slots:
    void insert_completion(const QString& completion);

signals:
    void enter_execute_event(QString);
};

#endif // COMMANDLINEEDIT_H
