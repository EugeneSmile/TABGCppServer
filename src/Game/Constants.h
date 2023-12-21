#ifndef GAME_CONSTANTS_H_GUARD
#define GAME_CONSTANTS_H_GUARD

#include <cstdint>

#include "Types.h"

constexpr uint16_t CHUNK_SIZE = 64;
// constexpr uint16_t PLAYER_CHUNK_DISTANCE = CHUNK_SIZE / 1.41;
constexpr uint8_t CHUNKS_AMOUNT = 64;
constexpr Vector2u16 WORLD_SIZE = {CHUNKS_AMOUNT * CHUNK_SIZE, CHUNKS_AMOUNT *CHUNK_SIZE};
constexpr Vector2u16 PLAYER_POSITION_LIMIT = {WORLD_SIZE.x - CHUNK_SIZE, WORLD_SIZE.y - CHUNK_SIZE};

#endif