#ifndef GAME_PHASES_COUNTDOWN_H_GUARD
#define GAME_PHASES_COUNTDOWN_H_GUARD

#include <chrono>

#include "GamePhase.h"
#include "Timer.h"

class Countdown : public GamePhase
{
private:
    Timer countdown_timer;
    void initialize();
    GameState process();

public:
    Countdown(/* args */);
    std::chrono::duration<float> countdown_time;
};

#endif