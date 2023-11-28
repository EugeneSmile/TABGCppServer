#include "Server.h"

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
    preferences = std::make_shared<Preferences>();
    network = std::make_shared<Network>();
    game = std::make_shared<Game>();
    players = std::make_shared<Players>();
    weapons = std::make_shared<Weapons>();
    cars = std::make_shared<Cars>();

    Logger::log->set_level(static_cast<spdlog::level::level_enum>(Config::getValue("level", 2, "Log")));
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
    while (*active)
    {
        std::chrono::high_resolution_clock::time_point tick_start = std::chrono::high_resolution_clock::now();

        network->process();

        std::chrono::nanoseconds tick_duration = std::chrono::high_resolution_clock::now() - tick_start;
        if (tick_duration < preferences->tick_time)
            std::this_thread::sleep_for(preferences->tick_time - tick_duration);
        else
            Logger::log->warn("Tick take too long: tick duration {}ns, limit: {}ns!", tick_duration.count(), preferences->tick_time.count());
    }
}
