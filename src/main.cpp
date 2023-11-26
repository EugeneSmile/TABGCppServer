#include <iostream>
#include <memory>
#include <signal.h>
#include <cstdint>

#include "Logger.h"
#include "Server.h"
#include "Version.h"

std::shared_ptr<bool> active = std::make_shared<bool>(true);

void signal_handler(int signal)
{
    Logger::log->warn("We received interruption signal, stopping");
    *active = false;
}

int main(int, char **)
{
    Logger::init();
    signal(SIGINT, signal_handler);
    Logger::log->info("Starting {} version {}", std::string(PROJECT_NAME), std::string(VERSION_FULL));

    std::shared_ptr<Server> server = Server::create();

    server->start(active);

    Logger::log->info("We are stopped. Bye!");
}
