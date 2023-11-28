#ifndef HELPERS_COUNTER_H_GUARD
#define HELPERS_COUNTER_H_GUARD

#include <chrono>

class Counter
{
private:
    std::chrono::nanoseconds period;
    std::chrono::high_resolution_clock::time_point timepoint_start;

public:
    Counter(std::chrono::seconds period);
    void start();
    std::chrono::seconds getSeconds();
    bool active{false};
};

#endif