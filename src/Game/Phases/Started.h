#ifndef GAME_PHASES_STARTED_H_GUARD
#define GAME_PHASES_STARTED_H_GUARD

#include <vector>
#include <cstdint>

#include "GamePhase.h"
#include "Ring.h"

class Started : public GamePhase
{
private:
    uint16_t first_ring_radius;
    uint16_t last_ring_radius;

    void prepareRings();
    void initialize();
    GameState process();

public:
    Started(/* args */);
    std::chrono::duration<float> preparation_time;
    std::chrono::duration<float> ring_time;
    std::vector<Ring> rings;
    uint32_t number_of_rings;
    size_t current_ring_index;
};

#endif