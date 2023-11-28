#ifndef NETWORK_PACKETHANDLER_H_GUARD
#define NETWORK_PACKETHANDLER_H_GUARD

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
    void sendMessageToPeer(ENetEvent *event, ClientEventCode code, Buffer &buffer, bool reliable);

public:
    void handle(ENetEvent *event);
};

#endif