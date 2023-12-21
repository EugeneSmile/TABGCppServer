#ifndef NETWORK_REQUESTS_H_GUARD
#define NETWORK_REQUESTS_H_GUARD

#include <functional>
#include <unordered_map>

#include <enet.h>

#include "ServerPtr.h"
#include "Enums.h"
#include "NetworkTypes.h"

class Packet;

typedef void RequestFunction(std::unique_ptr<Packet> &packet, void *ctx);
typedef std::function<RequestFunction> RequestFunctionPtr;

class Requests : public ServerPtr
{
public:
    std::unordered_map<ClientEventCode, RequestFunctionPtr> function;
    std::unordered_map<ClientEventCode, size_t> buffer_size;

    Requests();
    RequestFunction serverShutdown;
    RequestFunction initRoom;
    RequestFunction playerUpdate;
    RequestFunction login;
    RequestFunction playerLeft;
    RequestFunction playerRespawn;
    RequestFunction playerFire;
    RequestFunction gameStateChanged;
    RequestFunction weaponPickup;
    RequestFunction airplaneDrop;
    RequestFunction chatMessage;
    RequestFunction throwChatMessage;
    RequestFunction spawnGun;
    RequestFunction ringUpdate;
    RequestFunction startRegisterDamage;
    RequestFunction planeUpdate;
    RequestFunction planeAllDrop;
    RequestFunction playerLand;
    RequestFunction allWeapons;
    RequestFunction chunkEntry;
    RequestFunction chunkExit;
    RequestFunction chunkEntryOtherPlayer;
    RequestFunction chunkExitOtherPlayer;
};

#endif