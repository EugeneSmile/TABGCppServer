#ifndef GAME_PHASES_FLYING_H_GUARD
#define GAME_PHASES_FLYING_H_GUARD

#include <chrono>
#include <memory>
#include <unordered_set>

#include "GamePhase.h"
#include "Types.h"

class Player;

struct Plane
{
    Vector3f start;
    Vector3f finish;
    std::unordered_set<std::shared_ptr<Player>> players;
};

class Flying : public GamePhase
{
private:
    Timer flying_timer;
    void initialize();
    GameState process();

public:
    Flying(/* args */);
    void randomizePlane();
    Plane plane;
    std::chrono::duration<float> flight_time;
};

#endif