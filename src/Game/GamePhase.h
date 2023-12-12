#ifndef GAME_GAMEPHASE_H_GUARD
#define GAME_GAMEPHASE_H_GUARD

#include "ServerPtr.h"
#include "Timer.h"
#include "Enums.h"

class GamePhase : public ServerPtr
{
protected:
    Timer phase_time;
    bool initialized = false;
    virtual void initialize();
    virtual GameState process();

public:
    GameState tick();
};

#endif