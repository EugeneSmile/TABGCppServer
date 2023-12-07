#ifndef GAME_CHUNK_H_GUARD
#define GAME_CHUNK_H_GUARD

#include <cstdint>

struct Chunk
{
    uint8_t players;
    uint16_t loot;
    uint16_t cars;
};

#endif