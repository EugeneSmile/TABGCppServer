#ifndef GAME_PHASES_WAITING_H_GUARD
#define GAME_PHASES_WAITING_H_GUARD

#include "GamePhase.h"

class Waiting : public GamePhase
{
private:
    Timer timer;
    std::chrono::duration<float> time;
    void initialize();
    GameState process();

public:
    Waiting(/* args */);
};

#endif