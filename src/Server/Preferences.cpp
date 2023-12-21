#include "Preferences.h"

#include <initializer_list>

#include "Config.h"
#include "GeneralHelper.h"
#include "Version.h"

Preferences::Preferences()
{
    id = genUUID();
    name = Config::getValue("name", std::string(PROJECT_NAME), "Server");
    description = Config::getValue("description", std::string(PROJECT_NAME) + " TABG Server", "Server");
    max_players = Config::getValue("max_players", 64, "Server");
    group_size = Config::getValue("squad_size", 4, "Game");
    game_mode = Config::getValue("game_mode", "BattleRoyale", "Game");
    password = Config::getValue("password", "", "Server");
    lives = Config::getValue("lives", 2, "Game");
    kills = Config::getValue("kills", 0, "Game");
    tick_rate = Config::getValue("tickrate", 200, "Server");
    if (tick_rate)
        tick_time = std::chrono::nanoseconds(int64_t(1e9) / tick_rate);
    else
        tick_time = std::chrono::nanoseconds(0);
    start_time = std::chrono::high_resolution_clock::now();

    passworded = !password.empty();

    switch (group_size)
    {
    case 1:
        squad_mode = "SOLO";
        break;

    case 2:
        squad_mode = "DUO";
        break;

    default:
        group_size = 4;
    case 4:
        squad_mode = "SQUAD";
        break;
    }
}