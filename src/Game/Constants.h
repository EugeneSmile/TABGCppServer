#ifndef GAME_CONSTANTS_H_GUARD
#define GAME_CONSTANTS_H_GUARD

#include "Types.h"

constexpr Vector2u16 CHUNK_SIZE = {256, 256};
constexpr Vector2u8 CHUNKS_AMOUNT = {16, 16};
constexpr Vector2u16 WORLD_SIZE = {CHUNKS_AMOUNT.x * CHUNK_SIZE.x, CHUNKS_AMOUNT.y *CHUNK_SIZE.y};
constexpr Vector2u16 PLAYER_POSITION_LIMIT = {WORLD_SIZE.x - CHUNK_SIZE.x, WORLD_SIZE.y - CHUNK_SIZE.y};

#endif