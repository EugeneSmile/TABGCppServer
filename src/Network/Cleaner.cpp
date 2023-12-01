#include "Cleaner.h"

#include "Server.h"
#include "Logger.h"

Cleaner::Cleaner(/* args */) : period(std::chrono::seconds(Config::getValue("cleaner_period", 10, "Network"))), last_clean_timepoint(std::chrono::high_resolution_clock::now())
{
}

Cleaner::~Cleaner()
{
}

void Cleaner::tick()
{
    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - last_clean_timepoint) > period)
    {
        for (auto &peer : server->network->peers)
        {
            auto peer_state = enet_peer_get_state(peer.second);
            if (peer_state == ENetPeerState::ENET_PEER_STATE_DISCONNECTED || peer_state == ENetPeerState::ENET_PEER_STATE_ZOMBIE)
            {
                Logger::log->debug("Lost peer {}", peer.first);
                server->network->removePeer(peer.second->connectID);
            }
        }
        last_clean_timepoint = std::chrono::high_resolution_clock::now();
    }
}
