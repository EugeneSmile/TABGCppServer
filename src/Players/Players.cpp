#include "Players.h"

void Players::addPlayer(const Player &player)
{
    players.try_emplace(player.id, player);
}
void Players::removePlayer(const Player &player)
{
    players.erase(player.id);
}
void Players::updatePlayerLocation(uint8_t player_id, Vector3f new_location)
{
    if (players.contains(player_id))
        players.at(player_id).location = new_location;
}

uint8_t Players::increaseLastId()
{
    return ++last_id;
}