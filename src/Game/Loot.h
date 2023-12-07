#ifndef GAME_LOOT_H_GUARD
#define GAME_LOOT_H_GUARD

#include "ServerPtr.h"
#include "Types.h"

struct LootObject
{
    Vector3f position;
};

class Loot : public ServerPtr
{
private:
    /* data */
public:
    Loot(/* args */);
    ~Loot();
};

Loot::Loot(/* args */)
{
}

Loot::~Loot()
{
}

#endif