#ifndef SERVER_GAME_H_GUARD
#define SERVER_GAME_H_GUARD

#include "ServerPtr.h"
#include "Types.h"
#include "Enums.h"
#include "Counter.h"

#include <chrono>

struct Plane
{
    Vector3f start;
    Vector3f finish;
};

class Game : public ServerPtr
{
private:
    void randomizePlane();

public:
    Counter counter_waiting_for_players{std::chrono::seconds(10)};
    Counter counter_countdown{std::chrono::seconds(20)};
    Counter counter_flying{std::chrono::seconds(50)};
    Plane plane;
    GameState state{GameState::WaitingForPlayers};
    float countdown_time;
    float daytime = 0;
    uint8_t modifier = 0;

    Game(/* args */);
    void changeState(GameState state);
    float getCountdownCounter();
    void tick();
};

#endif