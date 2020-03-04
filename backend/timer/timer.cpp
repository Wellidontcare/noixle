#include "timer.h"

Timer::Timer(QObject *parent) :
    QObject(parent),
    start_time(std::chrono::high_resolution_clock::now())
{}

Timer::~Timer(){
    auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::high_resolution_clock::now() - start_time).count();
    emit measured_time(QString::number(time_taken)+ "mu");
}
