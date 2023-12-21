#include "Chunks.h"

#include "Server.h"
#include "Player.h"

Chunks::Chunks()
{
    for (uint8_t y = 0; y < CHUNKS_AMOUNT; ++y)
        for (uint8_t x = 0; x < CHUNKS_AMOUNT; ++x)
            chunks.push_back(std::make_shared<Chunk>(Vector2u8{x, y}));
}

std::shared_ptr<Chunk> Chunks::getChunk(uint8_t x, uint8_t y)
{
    return chunks[y * CHUNKS_AMOUNT + x];
}

std::shared_ptr<Chunk> Chunks::getChunk(Vector2u8 index)
{
    return chunks[index.y * CHUNKS_AMOUNT + index.x];
}

std::shared_ptr<Chunk> Chunks::getChunk(Vector3f position)
{
    auto chunk_idx = getChankIndexByPosition(position);
    return chunks[chunk_idx.y * CHUNKS_AMOUNT + chunk_idx.x];
}

void Chunks::playerUpdate(const std::shared_ptr<Player> &player)
{
    Vector2u8 chunk_index_new = getChankIndexByPosition(player->game.position);
    Vector2i8 chunk_change = {chunk_index_new.x - player->chunk->index.x, chunk_index_new.y - player->chunk->index.y};
    if (chunk_change.x || chunk_change.y)
    {
        std::shared_ptr<Chunk> chunk_new = getChunk(chunk_index_new);
        Logger::log->info("Player {} changed chunk: pos: {}x{} old: {}x{} new: {}x{} change: {}x{}", player->index, player->game.position.x, player->game.position.z, player->chunk->index.x, player->chunk->index.y, chunk_index_new.x, chunk_index_new.y, chunk_change.x, chunk_change.y);

        server->network->sendUnicast(player->client.peer, ClientEventCode::ChunkExit, reinterpret_cast<void *>(&player->index));
        player->chunk = chunk_new;
        server->network->sendUnicast(player->client.peer, ClientEventCode::ChunkEntry, reinterpret_cast<void *>(&player->index));
    }
}