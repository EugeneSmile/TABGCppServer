#include "Players.h"

#include <algorithm>

#include <enet.h>

#include "Server.h"
#include "Logger.h"

std::vector<std::shared_ptr<Player>>::iterator Players::addPlayer(ClientData &&client, bool bot)
{
    std::vector<std::shared_ptr<Player>>::iterator player_it = findPlayer(client.playfab_id);
    std::vector<std::shared_ptr<Player>>::iterator disconnected_it = findDisconnected(client.playfab_id);
    if (player_it == players.end() && disconnected_it == disconnected.end())
    {
        std::shared_ptr<Player> player = std::make_shared<Player>(players.size(), std::move(client));
        server->game->groups->addPlayerToGroup(player);
        player->chunk = server->game->chunks->getChunk(player->game.position);
        players.push_back(player);
        for (auto &distance : distances)
            distance.push_back(0);
        distances.push_back({});
    }
    else
    {
        if (disconnected_it != players.end())
        {
            (*disconnected_it)->client.peer = client.peer;
            players.insert(players.begin() + (*disconnected_it)->index, *disconnected_it);
            disconnected.erase(disconnected_it);
        }
        else
            Logger::log->warn("Player duplication");
    }
    return findPlayer(client.playfab_id);
}

std::vector<std::shared_ptr<Player>>::iterator Players::removePlayer(const ENetPeer *peer)
{
    auto player = findPlayer(peer);
    if (player != players.end())
    {
        server->network->sendBroadcast(ClientEventCode::PlayerLeft, reinterpret_cast<void *>((*player)->index));
        disconnected.push_back(*player);
        players.erase(player);
    }
    return player;
}

std::vector<std::shared_ptr<Player>>::iterator Players::findPlayer(const uint64_t playfab_id)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if ((*it)->client.playfab_id == playfab_id)
            return it;
    return players.end();
}

std::vector<std::shared_ptr<Player>>::iterator Players::findPlayer(const uint8_t game_index)
{
    if (game_index < players.size())
        return players.begin() + game_index;
    return players.end();
}

std::vector<std::shared_ptr<Player>>::iterator Players::findPlayer(const ENetPeer *peer)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        if ((*it)->client.peer == peer)
            return it;
    return players.end();
}

std::vector<std::shared_ptr<Player>>::iterator Players::findDisconnected(const uint64_t playfab_id)
{
    for (auto it = disconnected.begin(); it != disconnected.end(); ++it)
        if ((*it)->client.playfab_id == playfab_id)
            return it;
    return disconnected.end();
}

void Players::findDistances()
{
    for (size_t i = 0; i < distances.size(); ++i)
    {
        for (size_t j = 0; j < distances.at(i).size(); ++j)
        {
            Vector3f &pos1 = players.at(i)->game.position;
            Vector3f &pos2 = players.at(j + i + 1)->game.position;
            distances.at(i).at(j) = sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.z - pos1.z, 2));
        }
    }
}

float Players::getDistance(uint8_t index1, uint8_t index2)
{
    return distances.at(index1 < index2 ? index1 : index2).at(index1 > index2 ? index1 : index2);
}