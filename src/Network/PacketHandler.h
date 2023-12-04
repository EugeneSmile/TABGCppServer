#ifndef NETWORK_PACKETHANDLER_H_GUARD
#define NETWORK_PACKETHANDLER_H_GUARD

#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"
#include "Requests.h"
#include "Responses.h"

class Buffer;
class PacketHandler : public ServerPtr
{
private:
    Requests requests;
    Responses responses;

    std::vector<ClientEventCode> periodic_broadcast_codes;

    std::vector<ClientEventCode> packet_log_filter;
    void logPacket(const uint8_t *data, const size_t size, const std::string &header);

public:
    PacketHandler();
    void handleResponce(ENetEvent *event);
    void handleRequest(ENetPeer *peer, ClientEventCode code, Buffer *buffer, bool reliable);
    void doBroadcasts();
    void doRequest(ClientEventCode code, void *ctx = nullptr, ENetPeer *peer = nullptr);
};

#endif