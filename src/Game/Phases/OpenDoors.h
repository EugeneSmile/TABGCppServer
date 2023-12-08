#ifndef GAME_PHASES_OPENDOORS_H_GUARD
#define GAME_PHASES_OPENDOORS_H_GUARD

#include "GamePhase.h"

class OpenDoors : public GamePhase
{
private:
    void initialize();
    GameState process();

public:
    OpenDoors(/* args */);
};

#endif