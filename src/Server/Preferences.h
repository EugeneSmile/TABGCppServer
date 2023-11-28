#ifndef SERVER_PREFERENCES_H_GUARD
#define SERVER_PREFERENCES_H_GUARD

#include <string>
#include <cstdint>
#include <chrono>

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
    uint32_t tick_rate;
    std::chrono::nanoseconds tick_time;
    std::chrono::high_resolution_clock::time_point start_time;
    bool passworded = false;
    bool accepting_players = true;
};

#endif