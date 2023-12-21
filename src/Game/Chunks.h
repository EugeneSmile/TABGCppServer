#ifndef GAME_CHUNKS_H_GUARD
#define GAME_CHUNKS_H_GUARD

#include <vector>
#include <memory>
#include <cstdint>

#include "ServerPtr.h"
#include "Chunk.h"
#include "Types.h"
#include "Constants.h"
#include "GeneralHelper.h"

class Player;

class Chunks : public ServerPtr
{
private:
    std::vector<std::shared_ptr<Chunk>> chunks;

public:
    Chunks();
    std::shared_ptr<Chunk> getChunk(uint8_t x, uint8_t y);
    std::shared_ptr<Chunk> getChunk(Vector2u8 index);
    std::shared_ptr<Chunk> getChunk(Vector3f position);
    void populate();
    void playerUpdate(const std::shared_ptr<Player> &player);
};

#endif