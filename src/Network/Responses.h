#ifndef NETWORK_RESPONSES_H_GUARD
#define NETWORK_RESPONSES_H_GUARD

#include <functional>
#include <unordered_map>
#include <memory>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"

class Packet;

typedef void ResponseFunction(std::unique_ptr<Packet> &packet);
typedef std::function<ResponseFunction> ResponseFunctionPtr;

class Responses : public ServerPtr
{
public:
    std::unordered_map<ClientEventCode, ResponseFunctionPtr> function;

    Responses();
    ResponseFunction initRoom;
    ResponseFunction worldState;
    ResponseFunction playerUpdate;
    ResponseFunction playerFire;
    ResponseFunction airplaneDrop;
    ResponseFunction playerLand;
    ResponseFunction ringDeath;
    ResponseFunction playerDead;
    ResponseFunction syncProjectile;
    ResponseFunction curseCleanse;
    ResponseFunction respawnTeammate;
    ResponseFunction clickInteract;
    ResponseFunction healthState;
    ResponseFunction chatMessage;
    ResponseFunction throwChatMessage;
    ResponseFunction reviveState;
    ResponseFunction markerChange;
    ResponseFunction weaponPickup;
};

#endif