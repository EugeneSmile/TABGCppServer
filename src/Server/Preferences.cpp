#include "Preferences.h"

#include "Config.h"
#include "GeneralHelper.h"
#include "Version.h"

Preferences::Preferences()
{
    id = genUUID();
    name = Config::getValue("name", std::string(PROJECT_NAME), "Server");
    description = Config::getValue("description", std::string(PROJECT_NAME) + " TABG Server", "Server");
    uint8_t max_players = Config::getValue("max_players", 64, "Server");
    squad_mode = Config::getValue("squad_mode", "SOLO", "Server");
    game_mode = Config::getValue("game_mode", "BattleRoyale", "Game");
    password = Config::getValue("password", "", "Server");
    lives = Config::getValue("lives", 2, "Game");
    kills = Config::getValue("kills", 0, "Game");

    passworded = !password.empty();
}
