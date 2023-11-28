#include "Network.h"

#include <string>
#include <arpa/inet.h>

#include "Server.h"

Network::Network()
{
    if (enet_initialize())
    {
        Logger::log->error("ENet initialization failed, exiting");
        server->stop();
        return;
    }

    server_channels = Config::getValue("channels", 256, "Server");
    server_address = Config::getValue("address", "0.0.0.0", "Server");
    server_port = Config::getValue("port", 9700, "Server");

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

void Network::process()
{
    bool polled = false;

    while (!polled)
    {
        if (enet_host_check_events(host, event) <= 0)
        {
            if (enet_host_service(host, event, 15) <= 0)
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
            Logger::log->debug("Peer disconnected by timeout connect id {}, IP: {}", event->peer->connectID, inet_ntoa(event->peer->address.ipv4.ip));
            peers.erase(server->players->removePlayer(event->peer->connectID));
            break;

        case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
            packet_handler.handle(event);
            break;
        }
    }
}
