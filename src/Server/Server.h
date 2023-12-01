#ifndef SERVER_SERVER_H_GUARD
#define SERVER_SERVER_H_GUARD

#include <memory>
#include <chrono>
#include <thread>

#include "Logger.h"
#include "Config.h"
#include "ServerPtr.h"
#include "Players.h"
#include "Weapons.h"
#include "Cars.h"
#include "Network.h"
#include "Preferences.h"
#include "Game.h"

class Server : public std::enable_shared_from_this<Server>
{
private:
    void run();
    std::shared_ptr<bool> active;

public:
    std::shared_ptr<Preferences> preferences;
    std::shared_ptr<Game> game;
    std::shared_ptr<Network> network;
    std::shared_ptr<Players> players;
    std::shared_ptr<Weapons> weapons;
    std::shared_ptr<Cars> cars;

    void experimental_init();

    Server() = default;
    [[nodiscard]] static std::shared_ptr<Server> create();
    std::shared_ptr<Server> getPointer();
    void init();
    void deinit();
    void start(std::shared_ptr<bool> active);
    void stop();
    void restart();
};

#endif