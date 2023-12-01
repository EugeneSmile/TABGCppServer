#include "Network.h"

#include <chrono>
#include <string>
#include <arpa/inet.h>
#include <tuple>

#include "Server.h"

Network::Network()
{
    if (enet_initialize())
    {
        Logger::log->error("ENet initialization failed, exiting");
        server->stop();
        return;
    }

    address = new ENetAddress();
    event = new ENetEvent();

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

    Logger::log->info("Server is listening on {}:{} with {} channels", server_address, server_port, server_channels);

    if (Config::getValue("enabled", true, "Interface"))
        interface = std::make_shared<Interface>();
}

Network::~Network()
{
    enet_host_flush(host);
    enet_host_destroy(host);

    delete event;
    delete address;

    enet_deinitialize();
    Logger::log->info("Server stopped listening");
}

void Network::removePeer(uint32_t peer)
{
    auto player = server->players->removePlayer(peer);
    if (player != server->players->players.end())
    {
        peers.erase(player->service.playfab_id);
        std::tuple player_left_payload =
            {player->service.game_index, false};
        packet_handler.doRequest(ClientEventCode::PlayerLeft, static_cast<void *>(&player_left_payload));
    }
}

void Network::process()
{
    float poll_time_ratio = Config::getValue("poll_time_ratio", 0.2, "Server");
    bool polled = false;

    while (!polled)
    {
        if (enet_host_check_events(host, event) <= 0)
        {
            // Need to REALLY think about threading or so
            if (enet_host_service(host, event, std::ceil(poll_time_ratio * 1000 / server->preferences->tick_rate)) <= 0)
                break;

            polled = true;
        }

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
            removePeer(event->peer->connectID);
        }
        break;

        case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
            packet_handler.handleResponce(event);
            break;
        }
    }

    cleaner.tick();
    packet_handler.doBroadcasts();
}
