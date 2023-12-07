#ifndef GAME_GAME_H_GUARD
#define GAME_GAME_H_GUARD

#include <chrono>
#include <vector>

#include "ServerPtr.h"
#include "Types.h"
#include "Enums.h"
#include "Counter.h"
#include "Chunks.h"
#include "Ring.h"

constexpr std::chrono::duration<int64_t> FLIGHT_TIME = std::chrono::seconds(45);
constexpr Vector2u8 CHUNKS_AMOUNT = {16, 16};
constexpr Vector2u16 WORLD_SIZE = {CHUNKS_AMOUNT.x * CHUNK_SIZE.x, CHUNKS_AMOUNT.y *CHUNK_SIZE.y};
constexpr Vector2u16 PLAYER_POSITION_LIMIT = {WORLD_SIZE.x - CHUNK_SIZE.x, WORLD_SIZE.y - CHUNK_SIZE.y};

struct Plane
{
    Vector3f start;
    Vector3f finish;
};

class Game : public ServerPtr
{
private:
    uint16_t first_ring_radius;
    uint16_t last_ring_radius;
    void prepareRings();
    void randomizePlane();
    void keepPlayersInBounds();

    void stageWaiting();
    void stageCountdown();
    void stageFlying();
    void stageStarted();
    void stageEnded();

public:
    Counter counter_waiting_for_players{std::chrono::seconds(10)};
    Counter counter_countdown{std::chrono::seconds(20)};
    Counter counter_flying{FLIGHT_TIME};
    Plane plane;
    std::vector<Ring> rings;
    size_t current_ring_index{0};
    GameState state{GameState::WaitingForPlayers};
    float countdown_time;
    float daytime{0};
    float preparation_time{1000};
    float ring_time{1000};
    uint32_t number_of_rings{4};
    uint8_t modifier{0};

    Game(/* args */);
    void changeState(GameState state);
    float getCountdownCounter();
    void tick();
};

#endif