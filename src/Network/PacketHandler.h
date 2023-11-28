#ifndef NETWORK_PACKETHANDLER_H_GUARD
#define NETWORK_PACKETHANDLER_H_GUARD

#include <functional>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>

#include <enet.h>

#include "ServerPtr.h"
#include "Types.h"

class Buffer;
class PacketHandler : public ServerPtr
{
private:
    std::unordered_map<ClientEventCode, std::function<void(ENetEvent *, void *)>> functions;
    void sendMessageToPeer(ENetEvent *event, ClientEventCode code, Buffer &buffer, bool reliable);
    void reqInitRoom(ENetEvent *event, void *ctx = nullptr);
    void reqLogin(ENetEvent *event, void *ctx = nullptr);
    void reqPlayerUpdate(ENetEvent *event, void *ctx = nullptr);

public:
    PacketHandler();
    void handle(ENetEvent *event);
};

#endif