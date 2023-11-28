#ifndef SERVER_TYPES_H_GUARD
#define SERVER_TYPES_H_GUARD

#include <string>
#include <cstdint>

struct Vector3f
{
    float x;
    float y;
    float z;
};

struct LoginData
{
    std::string player_name;
    std::string gravestone_text;
    uint64_t login_key;
    bool squad_host;
    uint8_t squad_members;
    uint32_t gear_length;
};

struct PlayerServiceData
{
    uint64_t key;
    uint8_t game_index;
    uint32_t net_id;
    LoginData login_data;
};

#endif