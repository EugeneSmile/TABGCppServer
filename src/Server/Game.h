#ifndef SERVER_GAME_H_GUARD
#define SERVER_GAME_H_GUARD

#include "ServerPtr.h"
#include "Types.h"

#include <chrono>

struct Bus
{
    Vector3f start;
    Vector3f finish;
};

class Game : public ServerPtr
{
private:
    struct CountDown
    {
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::nanoseconds timer;
    } countdown;

    Bus bus;

    GameState state{GameState::WaitingForPlayers};

    void setBus();

public:
    Game(/* args */);
    ~Game();
    void changeState(GameState state);
    GameState getState();
    float getCountdownCounter();
    Bus getBus();
};

#endif