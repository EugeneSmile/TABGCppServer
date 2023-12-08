#include "GamePhase.h"

void GamePhase::initialize()
{
}
GameState GamePhase::process()
{
    return GameState::WaitingForPlayers;
}
GameState GamePhase::tick()
{
    if (!initialized)
    {
        initialize();
        phase_time.restart();
        initialized = true;
    }
    return process();
}
