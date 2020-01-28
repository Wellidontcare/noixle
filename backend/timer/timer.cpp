#include "timer.h"

Timer::Timer(QObject *parent) :
    QObject(parent),
    start_time(std::chrono::high_resolution_clock::now())
{
}

Timer::~Timer()
{
    emit meassured_time(QString::number
                        (std::chrono::duration_cast<std::chrono::microseconds>
                         (std::chrono::high_resolution_clock::now() - start_time).count())
                        + "mu");
}
