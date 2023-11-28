#include "Players.h"

#include <algorithm>

void Players::addPlayer(const uint64_t key, const Player &player)
{
    uint8_t index = players.size();
    indexes.emplace(key, index);
    players.push_back(player);
    in_game.push_back(index);
}
void Players::removePlayer(const uint8_t id)
{
    auto player = std::find(in_game.begin(), in_game.end(), id);
    if (player != in_game.end())
        in_game.erase(player);
}
void Players::updatePlayerLocation(uint8_t id, Vector3f new_location)
{
    players.at(id).location = new_location;
}

Player &Players::getPlayer(const uint8_t id)
{
    return players.at(id);
}

uint8_t Players::getNextFreeID()
{
    return players.size() + 1;
}

uint8_t Players::getPlayersCount()
{
    return players.size();
}