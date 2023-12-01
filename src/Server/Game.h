#ifndef SERVER_GAME_H_GUARD
#define SERVER_GAME_H_GUARD

#include "ServerPtr.h"
#include "Types.h"
#include "Enums.h"
#include "Counter.h"

#include <chrono>

struct Bus
{
    Vector3f start;
    Vector3f finish;
};

class Game : public ServerPtr
{
private:
    void randomizeBus();

public:
    Counter counterCountDown;
    Bus bus;
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