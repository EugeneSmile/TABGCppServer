#include "Server.h"

#include <random>

std::shared_ptr<Server> Server::create()
{
    std::shared_ptr<Server> server = std::make_shared<Server>();
    ServerPtr::init(server->getPointer());
    server->init();
    return server;
}

std::shared_ptr<Server> Server::getPointer()
{
    return shared_from_this();
}

void Server::init()
{
    timepoint_start = std::chrono::high_resolution_clock::now();

    preferences = std::make_unique<Preferences>();
    game = std::make_unique<Game>();
    network = std::make_unique<Network>();
#ifdef ENABLE_GUI
    gui = std::make_unique<Gui>();
#endif

    Logger::log->set_level(static_cast<spdlog::level::level_enum>(Config::getValue("level", 2, "Log")));
}

void Server::deinit()
{
#ifdef ENABLE_GUI
    gui.reset();
#endif
    game.reset();
    network.reset();
    preferences.reset();
}

void Server::start(std::shared_ptr<bool> active)
{
    this->active = active;
    run();
}
void Server::stop()
{
    *active = false;
}

void Server::restart()
{
}

void Server::run()
{
#ifdef ENABLE_GUI
    gui->start();
#endif

    network->startGetEvents();

    while (*active)
    {
        std::chrono::high_resolution_clock::time_point tick_start = std::chrono::high_resolution_clock::now();

        game->tick();
        network->process();

        tick_duration = std::chrono::high_resolution_clock::now() - tick_start;
        if (preferences->tick_rate)
        {
            if (tick_duration < preferences->tick_time)
                std::this_thread::sleep_for(preferences->tick_time - tick_duration);
            else
                Logger::log->warn("Tick take too long: tick duration {}ns, limit: {}ns!", tick_duration.count(), preferences->tick_time.count());
        }
    }

    network->finishGetEvents();
}

void Server::createMaBoi()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, 999);
    std::uniform_real_distribution<float> dis2(-512, 512);
    ClientData maboi_data;
    maboi_data.auto_squad = true;
    maboi_data.playfab_id = dis(gen);
    maboi_data.name = "Ma Boi " + std::to_string(maboi_data.playfab_id);
    maboi_data.gear = std::vector<uint32_t>(12, 0);
    auto player = game->players->addPlayer(std::move(maboi_data), true);
    (*player)->game.position = {dis2(gen), 115, dis2(gen)};
}
