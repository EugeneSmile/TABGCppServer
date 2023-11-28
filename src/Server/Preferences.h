#ifndef SERVER_PREFERENCES_H_GUARD
#define SERVER_PREFERENCES_H_GUARD

#include <string>
#include <cstdint>

class Preferences
{
public:
    Preferences();
    std::string id;
    std::string name;
    std::string description;
    uint8_t max_players;
    std::string squad_mode;
    std::string game_mode;
    std::string password;
    uint32_t lives;
    uint16_t kills;
    bool passworded = false;
    bool accepting_players = true;
};

#endif