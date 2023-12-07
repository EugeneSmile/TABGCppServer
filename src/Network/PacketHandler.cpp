#include "PacketHandler.h"

#include <chrono>

#include <magic_enum.hpp>

#include "Server.h"
#include "GeneralHelper.h"
#include "Buffer.h"

PacketHandler::PacketHandler()
{
    broadcasts = {
        {"PlayersUpdate", ClientEventCode::PlayerUpdate, nullptr, nullptr}};

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
    for (auto broadcast : broadcasts)
        requests.function[broadcast.code](broadcast.ctx, broadcast.peer);
}

void PacketHandler::doRequest(ClientEventCode code, void *ctx, ENetPeer *peer)
{
    requests.function[code](ctx, peer);
}

void PacketHandler::handleRequest(ENetPeer *peer, ClientEventCode code, Buffer *buffer, bool reliable)
{
    bool create_buffer = false;
    if (buffer == nullptr)
    {
        buffer = new Buffer(1);
        create_buffer = true;
    }
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

    if (create_buffer)
        delete buffer;
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
    else
        Logger::log->warn("Received not implemented response: {}", magic_enum::enum_name(code));
}

void PacketHandler::registerBroadcast(const std::string &name, ClientEventCode code, void *ctx, ENetPeer *peer)
{
    broadcasts.push_back({name, code, ctx, peer});
}

void PacketHandler::unregisterBroadcast(const std::string &name)
{
    for (std::vector<BroadcastData>::iterator it = broadcasts.begin(); it != broadcasts.end(); ++it)
        if (it->name == name)
            broadcasts.erase(it);
}