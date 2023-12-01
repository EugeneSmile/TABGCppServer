#ifndef NETWORK_REQUESTS_H_GUARD
#define NETWORK_REQUESTS_H_GUARD

#include <functional>
#include <unordered_map>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"

class Requests : public ServerPtr
{
public:
    std::unordered_map<ClientEventCode, std::function<void(void *ctx, ENetPeer *)>> function;

    Requests();
    void playerUpdate(void *ctx = nullptr, ENetPeer *peer = nullptr);
    void login(void *ctx = nullptr, ENetPeer *peer = nullptr);
    void playerLeft(void *ctx = nullptr, ENetPeer *peer = nullptr);
    void gameStateChanged(void *ctx = nullptr, ENetPeer *peer = nullptr);

    void spawnGun(void *ctx = nullptr, ENetPeer *peer = nullptr);
};

#endif