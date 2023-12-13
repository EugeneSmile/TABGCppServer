#ifndef GAME_PHASES_STARTED_H_GUARD
#define GAME_PHASES_STARTED_H_GUARD

#include <vector>
#include <cstdint>

#include "GamePhase.h"
#include "Ring.h"
#include "Timer.h"

class Started : public GamePhase
{
private:
    uint16_t first_ring_size;
    uint16_t last_ring_size;

    void prepareRings();
    void initialize();
    GameState process();

public:
    Started(/* args */);
    std::chrono::duration<float> ring_spawn_delay;
    std::chrono::duration<float> ring_base_time;
    Timer ring_timer;
    std::vector<Ring> rings;
    Ring *current_ring;
    uint32_t number_of_rings;
    size_t current_ring_index;
};

#endif