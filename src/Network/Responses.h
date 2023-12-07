#ifndef NETWORK_RESPONSES_H_GUARD
#define NETWORK_RESPONSES_H_GUARD

#include <functional>
#include <unordered_map>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"

typedef std::function<void(ENetEvent *)> ResponceFunction;

class Responses : public ServerPtr
{
public:
    std::unordered_map<ClientEventCode, std::function<void(ENetEvent *)>> function;

    Responses();
    void initRoom(ENetEvent *event);
    void worldState(ENetEvent *event);
    void playerUpdate(ENetEvent *event);
    void playerFire(ENetEvent *event);
    void airplaneDrop(ENetEvent *event);
    void playerLand(ENetEvent *event);
    void ringDeath(ENetEvent *event);
    void playerDead(ENetEvent *event);
    void syncProjectile(ENetEvent *event);
    void curseCleanse(ENetEvent *event);
    void respawnTeammate(ENetEvent *event);
    void clickInteract(ENetEvent *event);
    void healthState(ENetEvent *event);
    void chatMessage(ENetEvent *event);
    void throwChatMessage(ENetEvent *event);
    void reviveState(ENetEvent *event);
    void markerChange(ENetEvent *event);
};

#endif