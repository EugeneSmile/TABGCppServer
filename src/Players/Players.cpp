#include "Players.h"

void Players::addPlayer(const Player &player)
{
    players.push_back(player);
}
void Players::removePlayer(const size_t id)
{
    players.erase(std::next(players.begin(), id));
}
void Players::updatePlayerLocation(uint8_t id, Vector3f new_location)
{
    players.at(id).location = new_location;
}

Player &Players::getPlayer(const size_t id)
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