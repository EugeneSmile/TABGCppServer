#ifndef NETWORK_NETWORK_H_GUARD
#define NETWORK_NETWORK_H_GUARD

#include <enet.h>

#include "ServerPtr.h"
#include "PacketHandler.h"

constexpr uint16_t server_port = 9700;
constexpr size_t server_max_players = 256;

class Network : public ServerPtr
{
private:
    ENetHost *host;
    ENetEvent *event = new ENetEvent();
    ENetAddress *address = new ENetAddress();
    PacketHandler packet_handler;

public:
    Network();
    ~Network();
    void process();
};

#endif