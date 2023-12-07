#include "GeneralHelper.h"

#include <random>
#include <sstream>

#include "Game.h"
#include "Chunk.h"

const std::string genUUID()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++)
    {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++)
    {
        ss << dis(gen);
    };
    return ss.str();
}

bool floatsAreEqual(float a, float b)
{
    return (std::fabs(a - b) <= std::numeric_limits<float>::epsilon() * std::fmax(std::fabs(a), std::fabs(b)));
}

Vector2u8 getChankByPosition(Vector3f &position)
{
    return {
        static_cast<uint8_t>(static_cast<uint32_t>(position.x + WORLD_SIZE.x / 2) / CHUNK_SIZE.x), static_cast<uint8_t>(static_cast<uint32_t>(position.z + WORLD_SIZE.y / 2) / CHUNK_SIZE.y)};
}