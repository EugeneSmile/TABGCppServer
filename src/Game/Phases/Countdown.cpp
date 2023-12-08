#include "Countdown.h"

#include "Config.h"
#include "Logger.h"

Countdown::Countdown(/* args */)
{
    countdown_time = std::chrono::duration<float>(Config::getValue("countdown_time", 20, "Game"));
}

void Countdown::initialize()
{
    countdown_timer.restart();
}

GameState Countdown::process()
{
    if (countdown_timer.passed<std::chrono::seconds>())
        Logger::log->debug("Countdown phase: {}", (countdown_time - std::chrono::duration_cast<std::chrono::seconds>(countdown_timer.get())).count());
    if (countdown_timer.get() >= std::chrono::duration_cast<std::chrono::nanoseconds>(countdown_time))
        return GameState::Flying;
    return GameState::CountDown;
}
