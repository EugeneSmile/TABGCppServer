#include "Packet.h"

Packet::Packet(ENetEvent *event) : event(event), enet_packet(event->packet), buffer(event->packet->data, event->packet->dataLength), reliable(true)
{
    type = PacketType::CLIENT;
    code = buffer.getClientEventCode();
}

Packet::Packet(PacketType type, ClientEventCode code, size_t size, bool same_buffer, bool reliable) : type(type), code(code), event(nullptr), buffer(Buffer(code, size)), same_buffer(same_buffer), reliable(reliable)
{
}

Packet::~Packet()
{
    // enet_packet_dispose(enet_packet);
}

void Packet::createENetPacket()
{
    enet_packet = enet_packet_create(buffer.getData(), buffer.getUsedSize(), (reliable ? ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE : ENetPacketFlag::ENET_PACKET_FLAG_NONE) | (same_buffer ? ENetPacketFlag::ENET_PACKET_FLAG_NO_ALLOCATE : ENetPacketFlag::ENET_PACKET_FLAG_NONE));
}
