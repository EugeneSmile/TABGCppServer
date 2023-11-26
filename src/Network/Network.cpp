#include "Network.h"

#include "Server.h"

Network::Network()
{
    if (enet_initialize())
    {
        Logger::log->error("ENet initialization failed, exiting");
        server->stop();
        return;
    }

    if (enet_address_set_hostname(address, "127.0.0.1"))
    {
        Logger::log->info("Unable to map address, exiting");
        server->stop();
        return;
    }
    address->port = server_port;

    host = enet_host_create(address, server_max_players, 0, 0, 0, 0);
}

Network::~Network()
{
    enet_host_flush(host);
    enet_host_destroy(host);

    delete event;
    delete address;

    enet_deinitialize();
}

void Network::process()
{
    while (enet_host_service(host, event, 15) > 0)
    {
        switch (event->type)
        {
        default:
        case ENetEventType::ENET_EVENT_TYPE_NONE:
            break;

        case ENetEventType::ENET_EVENT_TYPE_CONNECT:
            Logger::log->info("Client connected - ID: {}, IP: {}", event->peer->incomingPeerID, event->peer->incomingPeerID);
            break;

        case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
            Logger::log->info("Client disconnected - ID: {}, IP: {}", event->peer->incomingPeerID, event->peer->incomingPeerID);
            break;

        case ENetEventType::ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            Logger::log->info("Client disconnected by timeout - ID: {}, IP: {}", event->peer->incomingPeerID, event->peer->incomingPeerID);
            break;

        case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
            Logger::log->info("Packed received - ID: {}, IP: {}", event->peer->incomingPeerID, event->peer->incomingPeerID);
            packet_handler.handle(event);
            break;
        }
    }
}