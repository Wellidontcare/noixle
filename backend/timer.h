#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QString>
#include <chrono>

class Timer : public QObject
{
    Q_OBJECT
    std::chrono::high_resolution_clock::time_point start_time;

public:
    explicit Timer(QObject *parent = nullptr);
    ~Timer();

signals:
    void meassured_time(QString);

public slots:
};

#endif // TIMER_H
