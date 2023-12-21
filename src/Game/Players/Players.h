#ifndef GAME_PLAYERS_PLAYERS_H_GUARD
#define GAME_PLAYERS_PLAYERS_H_GUARD

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <memory>

#include "ServerPtr.h"
#include "Types.h"
#include "Player.h"

class Players : public ServerPtr
{
private:
public:
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Player>> disconnected;
    // std::vector<uint8_t> connected;
    std::vector<std::vector<float>> distances;

    std::vector<std::shared_ptr<Player>>::iterator addPlayer(ClientData &&client, bool bot = false);
    std::vector<std::shared_ptr<Player>>::iterator removePlayer(const ENetPeer *peer);
    std::vector<std::shared_ptr<Player>>::iterator findPlayer(const uint64_t playfab_id);
    std::vector<std::shared_ptr<Player>>::iterator findPlayer(const uint8_t game_index);
    std::vector<std::shared_ptr<Player>>::iterator findPlayer(const ENetPeer *peer);
    std::vector<std::shared_ptr<Player>>::iterator findDisconnected(const uint64_t playfab_id);
    void findDistances();
    float getDistance(uint8_t index1, uint8_t index2);
};

#endif