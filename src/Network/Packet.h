#ifndef NETWORK_PACKET_H_GUARD
#define NETWORK_PACKET_H_GUARD

#include <enet.h>

#include "Enums.h"
#include "Buffer.h"

enum class PacketType
{
    BROADCAST = 0x0,
    MULTICAST = 0x1,
    UNICAST = 0x2,
    CLIENT = 0x3
};

class Packet
{
public:
    PacketType type;
    ClientEventCode code;
    ENetEvent *event;
    ENetPacket *enet_packet;
    Buffer buffer;
    bool same_buffer;
    bool reliable;
    Packet(ENetEvent *event);
    Packet(PacketType type = PacketType::BROADCAST, ClientEventCode code = ClientEventCode::NoCodeSet, size_t size = 128, bool same_buffer = false, bool reliable = true);
    ~Packet();
    void createENetPacket();
};

#endif