#ifndef PLAYERS_PLAYER_H_GUARD
#define PLAYERS_PLAYER_H_GUARD

#include <string>
#include <cstdint>
#include <unordered_map>

#include "Types.h"
#include "Enums.h"

enum class MovementType
{
    NoFlags = 0x0,
    Flag0 = 0x1,
    Flag1 = 0x2,
    Flag2 = 0x4,
    Flag3 = 0x8,
    Duck = 0x10,
    Lying = 0x20,
    Flag6 = 0x40,
    Jump = 0x80
};

struct Marker
{
    MarkerType type{MarkerType::Marker};
    Vector3f position{0, 0, 0};
    Vector3f normal{0, 0, 0};
    bool set{false};
};

struct PlayerServiceData
{
    uint64_t session_id;
    uint8_t game_index;
    uint32_t peer_id;
    std::string name;
    std::string gravestone_text;
    bool squad_host;
    uint8_t squad_members;
    uint8_t group_index;
    uint32_t gear_length;
    std::vector<uint32_t> gear;
    std::string item;
    std::string playfab_id;
    uint32_t color;
    bool auto_squad;
    std::string auth_player_id;
    bool connected{false};
    bool dev{false};
};

struct PlayerGameData
{
    float health{100};
    // Vector3f position{420, 135, 460};
    Vector3f position{0, 135, 0};
    Vector2f rotation{0, 0};
    Vector3f direction{0, 0, 0};
    bool aim_down_sight{false};
    MovementType movement_type{MovementType::NoFlags};
    std::unordered_map<MarkerType, Marker> markers{{MarkerType::Marker, {}}, {MarkerType::Ping, {}}};
    struct Firing
    {
        FiringMode mode{FiringMode::None};
        uint32_t ammo_type;
        std::vector<uint8_t> extra_data;
    } firing;
    struct Driving
    {
        bool is_driving{false};
        uint32_t car_id{0};
        uint32_t seat_id{0};
        DrivingState state{DrivingState::None};
    } driving;
};

struct Player
{
    PlayerServiceData service;
    PlayerGameData game;
    void kill();
};

#endif