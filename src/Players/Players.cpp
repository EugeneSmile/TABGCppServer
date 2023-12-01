#include "Players.h"

#include <algorithm>

#include "Logger.h"

std::vector<Player>::iterator Players::addPlayer(PlayerServiceData &service_data)
{
    if (findPlayer(service_data.playfab_id) == players.end())
    {
        service_data.game_index = players.size();
        service_data.connected = true;
        players.push_back({service_data, {}});
        connected.push_back(service_data.game_index);
    }
    else
    {
        auto player = findPlayer(service_data.peer_id);
        if (player == players.end())
        {
            player = findPlayer(service_data.playfab_id);
            connected.push_back(player->service.game_index);
            player->service.connected = true;
            player->service.peer_id = service_data.peer_id;
        }
        else
            Logger::log->warn("Player duplication");
    }
    return findPlayer(service_data.playfab_id);
}

std::vector<Player>::iterator Players::removePlayer(const uint32_t peer_id)
{
    auto player = findPlayer(peer_id);
    if (player != players.end())
    {
        player->service.connected = false;
        auto it = std::find(connected.begin(), connected.end(), player->service.game_index);
        if (it != connected.end())
        {
            connected.erase(it);
            return player;
        }
    }
    return players.end();
}

std::vector<Player>::iterator Players::findPlayer(const std::string &playfab_id)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->service.playfab_id == playfab_id)
            return it;
    return players.end();
}

std::vector<Player>::iterator Players::findPlayer(const uint8_t game_index)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->service.game_index == game_index)
            return it;
    return players.end();
}

std::vector<Player>::iterator Players::findPlayer(const uint32_t peer_id)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->service.peer_id == peer_id)
            return it;
    return players.end();
}

std::vector<Player>::iterator Players::findPlayer(const uint64_t key)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if (it->service.session_id == key)
            return it;
    return players.end();
}
