#ifndef GAME_CHUNKS_H_GUARD
#define GAME_CHUNKS_H_GUARD

#include <vector>
#include <cstdint>

#include "Chunk.h"
#include "Types.h"
#include "Constants.h"
#include "GeneralHelper.h"

class Chunks
{
private:
public:
    std::vector<std::vector<Chunk>> chunks;
    Chunk &getChunk(Vector2i8 index);
    Chunk &getChunk(Vector3f position);
};

#endif