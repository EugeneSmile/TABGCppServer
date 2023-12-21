#ifndef GAME_CHUNK_H_GUARD
#define GAME_CHUNK_H_GUARD

#include <cstdint>
#include <memory>
#include <vector>

#include "Types.h"

class Player;
class Car;
class Weapon;

struct Chunk
{
    Vector2u8 index;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Car>> cars;
    std::vector<std::shared_ptr<Weapon>> weapons;
    Chunk(Vector2u8 index);
};

#endif