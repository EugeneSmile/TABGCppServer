#ifndef NETWORK_PACKETHANDLER_H_GUARD
#define NETWORK_PACKETHANDLER_H_GUARD

#include <functional>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>
#include <any>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"

class Buffer;
class PacketHandler : public ServerPtr
{
private:
    std::unordered_map<ClientEventCode, std::function<void(ENetEvent *)>> func_responce;
    std::unordered_map<ClientEventCode, std::function<void(ENetPeer *, void *ctx)>> func_request;
    void sendMessageToPeer(ENetPeer *peer, ClientEventCode code, Buffer &buffer, bool reliable);

    void respInitRoom(ENetEvent *event);
    void respPlayerUpdate(ENetEvent *event);

    void reqPlayerUpdate(ENetPeer *peer, void *ctx);
    void reqLogin(ENetPeer *peer, void *ctx);
    void reqSpawnGun(ENetPeer *peer, void *ctx);

public:
    PacketHandler();
    void handle(ENetEvent *event);
    void callBroadcast(ClientEventCode code, void *ctx);
};

#endif