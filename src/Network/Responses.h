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
    void playerUpdate(ENetEvent *event);
    void playerFire(ENetEvent *event);
};

#endif