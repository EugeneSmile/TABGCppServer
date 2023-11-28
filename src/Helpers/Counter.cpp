#include "Counter.h"

Counter::Counter(std::chrono::seconds period) : period(std::chrono::duration_cast<std::chrono::nanoseconds>(period)), timepoint_start(std::chrono::high_resolution_clock::now())
{
}

void Counter::start()
{
    timepoint_start = std::chrono::high_resolution_clock::now();
    active = true;
}

std::chrono::seconds Counter::getSeconds()
{
    auto count = std::chrono::high_resolution_clock::now() - timepoint_start;
    return std::chrono::duration_cast<std::chrono::seconds>(count < period ? period - count : std::chrono::nanoseconds(0));
}