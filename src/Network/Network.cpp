#include "Network.h"

#include <chrono>
#include <string>
#include <arpa/inet.h>

#include <magic_enum.hpp>

#include "Server.h"
#include "NetworkTypes.h"
#include "Packet.h"

Network::Network()
{
    if (enet_initialize())
    {
        Logger::log->error("ENet initialization failed, exiting");
        server->stop();
        return;
    }

    address = new ENetAddress();

    server_channels = Config::getValue("channels", 256, "Server");
    server_address = Config::getValue("address", "0.0.0.0", "Server");
    server_port = Config::getValue("port", 9700, "Server");
    log_packets = Config::getValue("log_packets", false, "Network");
    if (log_packets)
        Logger::createPacketLogger();

    if (enet_address_set_hostname(address, server_address.c_str()))
    {
        Logger::log->info("Unable to map address, exiting");
        server->stop();
        return;
    }
    address->port = server_port;

    host = enet_host_create(address, server_channels, 0, 0, 0, 0);
    if (host)
        Logger::log->info("Server is listening on {}:{} with {} channels", server_address, server_port, server_channels);
    else
        Logger::log->error("Unable to start server on {}:{}", server_address, server_port);

    if (Config::getValue("enabled", true, "Interface"))
        interface = std::make_shared<Interface>();

    // broadcasts = {{"PlayersUpdate", ClientEventCode::PlayerUpdate, nullptr}};
    broadcasts = {};
    packet_log_filter = {};
}

Network::~Network()
{
    enet_host_flush(host);
    enet_host_destroy(host);

    delete address;

    enet_deinitialize();
    Logger::log->info("Server stopped listening");
}

void Network::logPacket(const uint8_t *data, const size_t size, const std::string &header)
{
    std::vector<uint8_t> vec(data, data + size);
    ClientEventCode code = static_cast<ClientEventCode>(vec.at(0));
    if (std::find(packet_log_filter.begin(), packet_log_filter.end(), code) != packet_log_filter.end() || packet_log_filter.empty())
        Logger::packet_log->info("{} {} {:n}", header, magic_enum::enum_name(code), spdlog::to_hex(vec));
}

void Network::prepareRequest(std::unique_ptr<Packet> &packet, void *ctx)
{
    if (requests.function.contains(packet->code))
        requests.function[packet->code](packet, ctx);
    packet->createENetPacket();
}

void Network::handleResponce(ENetEvent *event)
{
    std::unique_ptr<Packet> packet = std::make_unique<Packet>(event);
    if ((packet->code != ClientEventCode::TABGPing) && (packet->code != ClientEventCode::PlayerUpdate))
        Logger::log->debug("Handling packet with ClientEventCode {}", magic_enum::enum_name(packet->code));

    if (server->network->log_packets)
        logPacket(packet->buffer.getData(), packet->buffer.getSize(), "[S < C] [ ]");

    if (responses.function.contains(packet->code))
        responses.function[packet->code](packet);
    else
        Logger::log->warn("Received not implemented response: {}", magic_enum::enum_name(packet->code));
}

void Network::registerBroadcast(const std::string &name, ClientEventCode code, void *ctx)
{
    broadcasts.push_back({name, code, ctx});
}

void Network::unregisterBroadcast(const std::string &name)
{
    for (std::vector<BroadcastData>::iterator it = broadcasts.begin(); it != broadcasts.end(); ++it)
        if (it->name == name)
            broadcasts.erase(it);
}

void Network::informShutdown()
{
    sendBroadcast(ClientEventCode::ServerShutDown);
}

uint8_t Network::getConnectedSize()
{
    return static_cast<uint8_t>(enet_host_get_peers_count(host));
}

void Network::startGetEvents()
{
    thread_get_events = std::thread(&Network::threadGetEvents, this, std::ref(server), host, std::ref(events), std::ref(mutex_get_events));
}

void Network::finishGetEvents()
{
    informShutdown();
    if (thread_get_events.joinable())
        thread_get_events.join();
}

void Network::threadGetEvents(std::shared_ptr<Server> &server, ENetHost *host, std::queue<ENetEvent *> &events, std::mutex &mutex)
{
    Logger::log->info("Network thread started");
    float poll_time_ratio = Config::getValue("poll_time_ratio", 0.2, "Server");
    while (*server->active)
    {
        ENetEvent *event = new ENetEvent();
        if (enet_host_check_events(host, event) <= 0)
        {
            if (enet_host_service(host, event, std::ceil(poll_time_ratio * 1000 / server->preferences->tick_rate)) <= 0)
            {
                delete event;
                continue;
            }
        }
        events.push(event);
    }
    Logger::log->info("Network thread stopped");
}

void Network::sendUnicast(ENetPeer *peer, ClientEventCode code, void *ctx)
{
    std::unique_ptr<Packet> packet = std::make_unique<Packet>(PacketType::UNICAST, code, 16384);
    prepareRequest(packet, ctx);
    if (server->network->log_packets)
        logPacket(packet->buffer.getData(), packet->buffer.getUsedSize(), "[S > C] [U]");
    enet_peer_send(peer, 0, packet->enet_packet);
    enet_packet_dispose(packet->enet_packet);
}

void Network::sendMulticast(std::vector<ENetPeer *> &peers, ClientEventCode code, void *ctx)
{
    std::unique_ptr<Packet> packet = std::make_unique<Packet>(PacketType::MULTICAST, code, 16384, false);
    prepareRequest(packet, ctx);
    if (server->network->log_packets)
        logPacket(packet->buffer.getData(), packet->buffer.getUsedSize(), "[S > C] [M]");
    enet_host_broadcast_selective(host, 0, packet->enet_packet, peers.data(), peers.size());
}

void Network::sendBroadcast(ClientEventCode code, void *ctx)
{
    std::unique_ptr<Packet> packet = std::make_unique<Packet>(PacketType::BROADCAST, code, 16384, false);
    prepareRequest(packet, ctx);
    if (server->network->log_packets)
        logPacket(packet->buffer.getData(), packet->buffer.getUsedSize(), "[S > C] [B]");
    enet_host_broadcast(host, 0, packet->enet_packet);
}

void Network::process()
{
    for (; !events.empty(); events.pop())
    {
        ENetEvent *event = events.front();
        switch (event->type)
        {
        default:
        case ENetEventType::ENET_EVENT_TYPE_NONE:
            break;

        case ENetEventType::ENET_EVENT_TYPE_CONNECT:
            Logger::log->debug("Peer connected with connect id {}, IP: {}", event->peer->connectID, inet_ntoa(event->peer->address.ipv4.ip));
            break;

        case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
        case ENetEventType::ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
        {
            Logger::log->debug("Peer disconnected by timeout connect id {}, IP: {}", event->peer->connectID, inet_ntoa(event->peer->address.ipv4.ip));
            server->game->players->removePlayer(event->peer);
        }
        break;

        case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
            handleResponce(event);
            break;
        }
        delete event;
    }

    for (auto broadcast : broadcasts)
        sendBroadcast(broadcast.code, broadcast.ctx);
}
