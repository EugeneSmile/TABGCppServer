#ifndef SERVER_SERVER_H_GUARD
#define SERVER_SERVER_H_GUARD

#include <memory>
#include <chrono>
#include <thread>

#include "Logger.h"
#include "ServerPtr.h"
#include "Preferences.h"
#include "Players.h"
#include "Weapons.h"
#include "Network.h"

constexpr std::chrono::nanoseconds tick_time = std::chrono::nanoseconds(16666667);

class Server : public std::enable_shared_from_this<Server>
{
private:
    void run();
    std::shared_ptr<bool> active;

public:
    std::shared_ptr<Preferences> preferences;
    std::shared_ptr<Network> network;
    std::shared_ptr<Players> players;
    std::shared_ptr<Weapons> weapons;

    Server() = default;
    [[nodiscard]] static std::shared_ptr<Server> create();
    static void deleter(Server *server);
    std::shared_ptr<Server> getPointer();
    void init();
    void start(std::shared_ptr<bool> active);
    void stop();
    void restart();
    void destroy();
};

#endif