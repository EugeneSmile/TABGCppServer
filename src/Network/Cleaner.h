#ifndef NETWORK_CLEANER_H_GUARD
#define NETWORK_CLEANER_H_GUARD

#include <chrono>
#include <cstdint>

#include <enet.h>

#include "ServerPtr.h"
#include "Config.h"

class Cleaner : public ServerPtr
{
private:
    std::chrono::seconds period;
    std::chrono::high_resolution_clock::time_point last_clean_timepoint;

public:
    Cleaner(/* args */);
    ~Cleaner();
    void tick();
};

#endif