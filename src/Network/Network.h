#ifndef NETWORK_NETWORK_H_GUARD
#define NETWORK_NETWORK_H_GUARD

#include <memory>
#include <unordered_map>

#include <enet.h>

#include "ServerPtr.h"
#include "PacketHandler.h"
#include "Interface.h"
#include "Cleaner.h"

class Network : public ServerPtr
{
private:
    ENetHost *host;
    ENetEvent *event;
    ENetAddress *address;
    std::shared_ptr<Interface> interface;
    int64_t server_channels;
    Cleaner cleaner;

public:
    Network();
    ~Network();
    void process();
    void removePeer(uint32_t peer_id);
    void informShutdown();
    PacketHandler packet_handler;
    std::unordered_map<std::string, ENetPeer *> peers;
    std::string server_address;
    int64_t server_port;
    bool log_packets{false};
};

#endif