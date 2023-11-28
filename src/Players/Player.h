#ifndef PLAYERS_PLAYER_H_GUARD
#define PLAYERS_PLAYER_H_GUARD

#include <string>
#include <cstdint>

#include "Types.h"

struct Player
{
    uint8_t group;
    std::string name;
    float health{100};
    Vector3f location{0, 100, 0};
    Vector2f rotation{0, 0};
    bool dev{false};
    bool is_in_car{false};
    uint32_t in_car_id{0};
    uint32_t in_car_seat{0};
    Player(const LoginData &data);
};

#endif