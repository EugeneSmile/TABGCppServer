#include "Timer.h"

Timer::Timer() : timepoint_start(std::chrono::high_resolution_clock::now()), timepoint_stop(std::chrono::high_resolution_clock::now()), timepoint_last(std::chrono::high_resolution_clock::now()), active(true) {}

void Timer::restart()
{
    timepoint_start = std::chrono::high_resolution_clock::now();
    timepoint_stop = std::chrono::high_resolution_clock::now();
    active = true;
}

void Timer::resume()
{
    timepoint_start = std::chrono::high_resolution_clock::now() - timepoint_stop + timepoint_start;
    active = true;
}

std::chrono::nanoseconds Timer::stop()
{
    timepoint_stop = std::chrono::high_resolution_clock::now();
    active = false;
    return timepoint_stop - timepoint_start;
}

std::chrono::nanoseconds Timer::get()
{
    return (active ? std::chrono::high_resolution_clock::now() - timepoint_start : timepoint_stop - timepoint_start);
}