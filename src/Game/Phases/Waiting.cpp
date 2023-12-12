#include "Waiting.h"

#include "Server.h"
#include "Config.h"
#include "Logger.h"

Waiting::Waiting(/* args */)
{
    time = std::chrono::duration<float>(Config::getValue("waiting_time", 20, "Game"));
}

void Waiting::initialize()
{
}

GameState Waiting::process()
{
    if (server->players->connected.size() > 0 && server->groups->groups.size() > 1)
        timer.resume();
    else
        timer.stop();
    if (timer.passed<std::chrono::seconds>())
        Logger::log->debug("Waiting for players time: {} / {}", std::chrono::duration_cast<std::chrono::seconds>(timer.get()).count(), time.count());
    if (timer.get() >= std::chrono::duration_cast<std::chrono::nanoseconds>(time))
        return GameState::CountDown;
    return GameState::WaitingForPlayers;
}
