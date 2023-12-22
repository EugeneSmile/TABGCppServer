#ifndef GAME_GAME_H_GUARD
#define GAME_GAME_H_GUARD

#include <chrono>
#include <vector>
#include <unordered_map>
#include <memory>

#include "ServerPtr.h"
#include "Types.h"
#include "Enums.h"
#include "GamePhase.h"

#include "Players.h"
#include "Groups.h"
#include "Weapons.h"
#include "Cars.h"
#include "Chunks.h"

#include "Waiting.h"
#include "Countdown.h"
#include "Flying.h"
#include "Started.h"
#include "Ended.h"
#include "OpenDoors.h"

class Game : public ServerPtr
{
private:
    void keepPlayersInBounds();
    void experimental_init();

public:
    std::shared_ptr<Players> players;
    std::shared_ptr<Groups> groups;
    std::shared_ptr<Weapons> weapons;
    std::shared_ptr<Cars> cars;
    std::shared_ptr<Chunks> chunks;

    std::unordered_map<GameState, std::shared_ptr<GamePhase>> phases;
    GameState state{GameState::WaitingForPlayers};
    float daytime{0};
    uint8_t modifier{0};
    bool spawn_loot_on_start{false};
    float player_distance_limit{1024};

    Game();
    void changeState(GameState state, bool force = false);
    void tick();

    template <typename T>
    std::shared_ptr<T> getPhase(GameState state)
    {
        return dynamic_pointer_cast<T>(phases.at(state));
    };
};

#endif