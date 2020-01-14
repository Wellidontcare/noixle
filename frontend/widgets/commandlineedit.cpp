#include "commandlineedit.h"

CommandLineEdit::CommandLineEdit(QWidget* parent)
    : QLineEdit(parent){}

void CommandLineEdit::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()){
    case Qt::Key::Key_Tab:{
        QModelIndex index = completer->currentIndex();
        completer->popup()->setCurrentIndex(index);
        int start = completer->currentRow();
        if(!completer->setCurrentRow(start + 1)){
            completer->setCurrentRow(0);
            return;
        }
        return;
    }
    case Qt::Key::Key_Return:{
        emit enter_execute_event(text());
        setText("");
        return;
    }
    default:{
        QLineEdit::keyPressEvent(event);
        return;
    }
    }
}

bool CommandLineEdit::event(QEvent *event){
    if(event->type() == QEvent::KeyPress){
        if(static_cast<QKeyEvent*>(event)->key() == Qt::Key::Key_Tab){
            keyPressEvent(static_cast<QKeyEvent*>(event));
            return true;
        }
        return QLineEdit::event(event);
    }
    return QLineEdit::event(event);
}

void CommandLineEdit::populate_options(QStringList list)
{
    available_options = list;
    completer = new QCompleter(available_options);
    setCompleter(completer);
}

void CommandLineEdit::insert_completion(const QString &completion){
    if(completer->widget() != this){
        return;
    }
}

