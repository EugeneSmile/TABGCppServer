#ifndef GAME_GAME_H_GUARD
#define GAME_GAME_H_GUARD

#include <chrono>
#include <vector>
#include <unordered_map>
#include <memory>

#include "ServerPtr.h"
#include "Types.h"
#include "Enums.h"
#include "Chunks.h"
#include "GamePhase.h"

#include "Phases/Waiting.h"
#include "Phases/Countdown.h"
#include "Phases/Flying.h"
#include "Phases/Started.h"
#include "Phases/Ended.h"
#include "Phases/OpenDoors.h"

class Game : public ServerPtr
{
private:
    void keepPlayersInBounds();

public:
    std::unordered_map<GameState, std::shared_ptr<GamePhase>> phases;
    GameState state{GameState::WaitingForPlayers};
    float daytime{0};

    uint8_t modifier{0};

    Game(/* args */);
    void changeState(GameState state);
    void tick();
};

#endif