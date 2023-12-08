#ifndef HELPERS_TIMER_H_GUARD
#define HELPERS_TIMER_H_GUARD

#include <chrono>

#include "TypeTraits.h"

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point timepoint_start;
    std::chrono::high_resolution_clock::time_point timepoint_stop;
    std::chrono::high_resolution_clock::time_point timepoint_last;
    bool active;

public:
    Timer();
    void restart();
    void resume();
    std::chrono::nanoseconds stop();
    std::chrono::nanoseconds get();
    template <typename T>
        requires IsTime<T>
    bool passed(int value = 1)
    {
        bool passed = std::chrono::high_resolution_clock::now() - timepoint_last > T(value);
        if (passed)
            timepoint_last = std::chrono::high_resolution_clock::now();
        return passed;
    }
};

#endif