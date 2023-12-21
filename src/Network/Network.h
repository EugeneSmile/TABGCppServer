#ifndef NETWORK_NETWORK_H_GUARD
#define NETWORK_NETWORK_H_GUARD

#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include <enet.h>

#include "ServerPtr.h"
#include "Interface.h"
#include "Requests.h"
#include "Responses.h"

struct BroadcastData
{
    std::string name;
    ClientEventCode code;
    void *ctx;
};

class Network : public ServerPtr
{
private:
    void threadGetEvents(std::shared_ptr<Server> &server, ENetHost *host, std::queue<ENetEvent *> &events, std::mutex &mutex);
    void logPacket(const uint8_t *data, const size_t size, const std::string &header);
    void prepareRequest(std::unique_ptr<Packet> &packet, void *ctx);
    void handleResponce(ENetEvent *event);
    ENetHost *host;
    ENetAddress *address;
    std::queue<ENetEvent *> events;
    std::thread thread_get_events;
    std::mutex mutex_get_events;
    std::shared_ptr<Interface> interface;
    int64_t server_channels;
    Requests requests;
    Responses responses;
    std::vector<BroadcastData> broadcasts;
    std::vector<ClientEventCode> packet_log_filter;

public:
    Network();
    ~Network();

    void process();
    void informShutdown();
    uint8_t getConnectedSize();

    void startGetEvents();
    void finishGetEvents();

    void sendUnicast(ENetPeer *peer, ClientEventCode code, void *ctx = nullptr);
    void sendMulticast(std::vector<ENetPeer *> &peers, ClientEventCode code, void *ctx = nullptr);
    void sendBroadcast(ClientEventCode code, void *ctx = nullptr);

    void registerBroadcast(const std::string &name, ClientEventCode code, void *ctx = nullptr);
    void unregisterBroadcast(const std::string &name);

    std::string server_address;
    int64_t server_port;
    bool log_packets{false};
};

#endif