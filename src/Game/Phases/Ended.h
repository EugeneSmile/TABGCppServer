#ifndef GAME_PHASES_ENDED_H_GUARD
#define GAME_PHASES_ENDED_H_GUARD

#include "GamePhase.h"

class Ended : public GamePhase
{
private:
    void initialize();
    GameState process();

public:
    Ended(/* args */);
};

#endif