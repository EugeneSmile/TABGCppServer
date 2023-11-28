#include "Players.h"

#include <algorithm>

#include "Logger.h"

void Players::addPlayer(const uint32_t net_id, const LoginData &data)
{
    if (!game_index.contains(data.login_key))
    {
        uint8_t index = players.size();
        game_index.emplace(data.login_key, index);
        net_session_id.emplace(net_id, data.login_key);
        login_data.emplace(data.login_key, data);
        players.push_back(Player(data));
        in_game.push_back(index);
    }
    else
    {
        if (net_session_id.find(net_id) == net_session_id.end())
            in_game.push_back(game_index[data.login_key]);
        else
            Logger::log->warn("Player duplication");
    }
}
void Players::removePlayer(const uint32_t net_id)
{
    auto player = std::find(in_game.begin(), in_game.end(), game_index[net_session_id[net_id]]);
    if (player != in_game.end())
        in_game.erase(player);
}
void Players::updatePlayerLocation(uint8_t id, Vector3f new_location)
{
    players.at(id).location = new_location;
}

Player &Players::getPlayer(const uint8_t id)
{
    return players[id];
}

Player &Players::getPlayer(const uint64_t key)
{
    return players[game_index[key]];
}

uint8_t Players::getPlayersCount()
{
    return in_game.size();
}

uint8_t Players::getGameIndex(uint64_t key)
{
    return game_index[key];
}

LoginData &Players::getLoginData(uint64_t key)
{
    return login_data[key];
}