#include "PacketHandler.h"

#include <chrono>

#include <magic_enum.hpp>

#include "Server.h"
#include "GeneralHelper.h"
#include "Buffer.h"

PacketHandler::PacketHandler()
{
    periodic_broadcast_codes = {
        ClientEventCode::PlayerUpdate};

    // packet_log_filter = {ClientEventCode::RoomInit, ClientEventCode::Login, ClientEventCode::PlayerLeft, ClientEventCode::RequestAirplaneDrop, ClientEventCode::PlayerAirplaneDropped};
    packet_log_filter = {ClientEventCode::PlayerUpdate};
}

void PacketHandler::logPacket(const uint8_t *data, const size_t size, const std::string &header)
{
    std::vector<uint8_t> vec(data, data + size);
    ClientEventCode code = static_cast<ClientEventCode>(vec.at(0));
    if (std::find(packet_log_filter.begin(), packet_log_filter.end(), code) != packet_log_filter.end() || packet_log_filter.empty())
        Logger::packet_log->info("{} {} {:n}", header, magic_enum::enum_name(code), spdlog::to_hex(vec));
}

void PacketHandler::doBroadcasts()
{
    for (auto code : periodic_broadcast_codes)
        requests.function[code](nullptr, nullptr);
}

void PacketHandler::doRequest(ClientEventCode code, void *ctx, ENetPeer *peer)
{
    requests.function[code](ctx, peer);
}

void PacketHandler::handleRequest(ENetPeer *peer, ClientEventCode code, Buffer *buffer, bool reliable)
{
    buffer->setClientEventCode(code);
    ENetPacket *packet = enet_packet_create(*buffer, buffer->getUsedSize(), (reliable ? ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE : 0));
    if (peer == nullptr)
    {
        for (auto peer_to_send : server->network->peers)
            enet_peer_send(peer_to_send.second, 0, packet);
    }
    else
    {
        enet_peer_send(peer, 0, packet);
    }
    enet_packet_dispose(packet);

    if (server->network->log_packets)
    {
        if (peer == nullptr)
        {
            if (!server->network->peers.empty())
                logPacket(buffer->getData(), buffer->getUsedSize(), "SENDB");
        }
        else
        {
            logPacket(buffer->getData(), buffer->getUsedSize(), "SENDP");
        }
    }
}

void PacketHandler::handleResponce(ENetEvent *event)
{
    ClientEventCode code = static_cast<ClientEventCode>(event->packet->data[0]);
    if ((code != ClientEventCode::TABGPing) && (code != ClientEventCode::PlayerUpdate))
        Logger::log->debug("Handling packet with ClientEventCode {}", magic_enum::enum_name(code));

    if (server->network->log_packets)
    {
        Buffer buffer = Buffer(event->packet->data, event->packet->dataLength);
        logPacket(buffer.getData(), buffer.getSize(), "RECVD");
    }

    if (responses.function.contains(code))
        responses.function[code](event);
}
