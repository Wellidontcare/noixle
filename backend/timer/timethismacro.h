#ifndef TIMETHISMACRO_H
#define TIMETHISMACRO_H
#define TIME_THIS Timer timer; connect(&timer, &Timer::meassured_time, this, &Backend::show_performance_info);
#endif // TIMETHISMACRO_H
