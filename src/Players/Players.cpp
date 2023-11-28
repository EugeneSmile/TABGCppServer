#include "Players.h"

#include <algorithm>

#include "Logger.h"

std::vector<std::pair<PlayerServiceData, Player>>::iterator Players::addPlayer(const uint32_t net_id, const LoginData &data)
{
    if (findPlayer(data.login_key) == players.end())
    {
        uint8_t index = players.size();
        PlayerServiceData player_data;
        player_data.key = data.login_key;
        player_data.game_index = index;
        player_data.net_id = net_id;
        player_data.login_data = data;
        players.push_back({player_data, Player(data)});
        connected.push_back(index);
    }
    else
    {
        auto player = findPlayer(net_id);
        if (player == players.end())
        {
            connected.push_back(player->first.game_index);
            player->first.net_id = net_id;
        }
        else
            Logger::log->warn("Player duplication");
    }
    return findPlayer(data.login_key);
}
uint64_t Players::removePlayer(const uint32_t net_id)
{
    auto player = findPlayer(net_id);
    if (player != players.end())
    {
        auto it = std::find(connected.begin(), connected.end(), player->first.game_index);
        if (it != connected.end())
        {
            connected.erase(it);
            return player->first.key;
        }
    }
    return 0;
}

std::vector<std::pair<PlayerServiceData, Player>>::iterator Players::findPlayer(const uint8_t game_index)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->first.game_index == game_index)
            return it;
    return players.end();
}
std::vector<std::pair<PlayerServiceData, Player>>::iterator Players::findPlayer(const uint32_t net_id)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->first.net_id == net_id)
            return it;
    return players.end();
}
std::vector<std::pair<PlayerServiceData, Player>>::iterator Players::findPlayer(const uint64_t key)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->first.key == key)
            return it;
    return players.end();
}

void Players::updatePlayerLocation(uint8_t id, Vector3f new_location)
{
    findPlayer(id)->second.location = new_location;
}
