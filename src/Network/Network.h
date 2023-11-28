#ifndef NETWORK_NETWORK_H_GUARD
#define NETWORK_NETWORK_H_GUARD

#include <memory>

#include <enet.h>

#include "ServerPtr.h"
#include "PacketHandler.h"
#include "Interface.h"

class Network : public ServerPtr
{
private:
    ENetHost *host;
    ENetEvent *event = new ENetEvent();
    ENetAddress *address = new ENetAddress();
    std::shared_ptr<Interface> interface;
    PacketHandler packet_handler;
    int64_t server_channels;

public:
    Network();
    ~Network();
    void process();
    std::string server_address;
    int64_t server_port;
};

#endif