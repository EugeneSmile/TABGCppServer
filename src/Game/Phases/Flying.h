#ifndef GAME_PHASES_FLYING_H_GUARD
#define GAME_PHASES_FLYING_H_GUARD

#include <chrono>

#include "GamePhase.h"
#include "Types.h"

struct Plane
{
    Vector3f start;
    Vector3f finish;
};

class Flying : public GamePhase
{
private:
    Timer flying_timer;
    void initialize();
    GameState process();
    void randomizePlane();

public:
    Flying(/* args */);
    Plane plane;
    const std::chrono::duration<float> flight_time;
};

#endif