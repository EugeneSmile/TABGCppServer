#ifndef NETWORK_PACKETHANDLER_H_GUARD
#define NETWORK_PACKETHANDLER_H_GUARD

#include <iostream>
#include <memory>
#include <vector>

#include <enet.h>

#include "ServerPtr.h"
#include "Types.h"

class PacketHandler : public ServerPtr
{
private:
    void createJoinMessage(std::vector<uint8_t> &data, uint8_t player_index, const std::string &player_name);
    void sendMessageToPeer(ENetEvent *event, ClientEventCode code, const std::vector<uint8_t> &data, bool reliable);

public:
    void handle(ENetEvent *event);
};

#endif