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
    groups = std::make_shared<Groups>();
    weapons = std::make_shared<Weapons>();
    cars = std::make_shared<Cars>();

    Logger::log->set_level(static_cast<spdlog::level::level_enum>(Config::getValue("level", 2, "Log")));

    // experimental_init();
}

void Server::deinit()
{
    cars.reset();
    weapons.reset();
    groups.reset();
    players.reset();
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
    while (*active)
    {
        std::chrono::high_resolution_clock::time_point tick_start = std::chrono::high_resolution_clock::now();

        game->tick();
        network->process();

        std::chrono::nanoseconds tick_duration = std::chrono::high_resolution_clock::now() - tick_start;
        if (tick_duration < preferences->tick_time)
            std::this_thread::sleep_for(preferences->tick_time - tick_duration);
        else
            Logger::log->warn("Tick take too long: tick duration {}ns, limit: {}ns!", tick_duration.count(), preferences->tick_time.count());
    }
    network->informShutdown();
}

void Server::experimental_init()
{
    PlayerServiceData experimental_player_data;
    experimental_player_data.name = "Steve";
    experimental_player_data.session_id = 754;
    experimental_player_data.playfab_id = "Steve";
    experimental_player_data.gear = std::vector<uint32_t>(12, 0);
    experimental_player_data.gear_length = 12;
    experimental_player_data.peer_id = 0;
    auto player = players->addPlayer(experimental_player_data);
    player->service.connected = false;
    player->game.position = {-20, 115, -20};

    cars->addCar({1, 0, {0, 115, -10}});
    cars->addCar({1, 1, {10, 115, -10}});
    cars->addCar({1, 2, {20, 115, -10}});
    cars->addCar({1, 3, {30, 115, -10}});
    cars->addCar({1, 4, {40, 115, -10}});
    cars->addCar({1, 5, {50, 115, -10}});
    cars->addCar({1, 6, {60, 115, -10}});
}