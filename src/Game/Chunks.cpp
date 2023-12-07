#include "Chunks.h"

Chunk &Chunks::getChunk(Vector2i8 index)
{
    return chunks[index.x][index.y];
}

Chunk &Chunks::getChunk(Vector3f position)
{
    auto chunk_idx = getChankByPosition(position);
    return chunks[chunk_idx.x][chunk_idx.y];
}
