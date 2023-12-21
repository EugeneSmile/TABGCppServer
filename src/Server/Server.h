#ifndef SERVER_SERVER_H_GUARD
#define SERVER_SERVER_H_GUARD

#include <memory>
#include <chrono>
#include <thread>

#include "Logger.h"
#include "Config.h"
#include "ServerPtr.h"
#include "Network.h"
#include "Preferences.h"
#include "Game.h"
#ifdef ENABLE_GUI
#include "Gui.h"
#endif

class Server : public std::enable_shared_from_this<Server>
{
private:
    void createMaBoi();
    void run();

public:
    std::shared_ptr<bool> active;
    std::unique_ptr<Preferences> preferences;
    std::unique_ptr<Game> game;
    std::unique_ptr<Network> network;
#ifdef ENABLE_GUI
    std::unique_ptr<Gui> gui;
#endif
    std::chrono::nanoseconds tick_duration;
    std::chrono::high_resolution_clock::time_point timepoint_start;
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