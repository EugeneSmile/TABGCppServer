#ifndef GAME_PLAYERS_PLAYER_H_GUARD
#define GAME_PLAYERS_PLAYER_H_GUARD

#include <string>
#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>

#include <enet.h>

#include "Types.h"
#include "Enums.h"
#include "Weapon.h"

class Group;
class Car;
class Chunk;

constexpr uint8_t INVENTORY_SIZE = 16;
constexpr uint8_t BLESSINGS_SIZE = 3;
constexpr uint8_t THROWABLES_SIZE = 3;

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
    Vector3f normal;
    Vector3f position;
    MarkerType type;
    bool set;
};

struct ClientData
{
    std::string auth_player_id;
    bool auto_squad;
    uint32_t color;
    bool dev;
    std::vector<uint32_t> gear;
    std::string gravestone_text;
    std::shared_ptr<Group> group;
    bool group_host;
    uint8_t group_size;
    std::string item;
    std::string name;
    uint64_t playfab_id;
    uint64_t session_id;
    ENetPeer *peer;
};

struct GameData
{
    bool aim_down_sight;
    uint8_t curse_id;
    Vector3f direction;
    bool flying;
    float health;
    bool knocked;
    std::unordered_map<MarkerType, Marker> markers;
    MovementType movement_type;
    Vector3f position;
    Vector2f rotation;
    TABGPlayerState state;
};

struct Inventory
{
    std::shared_ptr<Weapon> equiped;
    std::vector<std::shared_ptr<Weapon>> backpack;
    std::vector<std::shared_ptr<Weapon>> blessings;
    struct Slots
    {
        std::pair<std::shared_ptr<Weapon>, std::shared_ptr<Weapon>> a;
        std::pair<std::shared_ptr<Weapon>, std::shared_ptr<Weapon>> b;
        std::shared_ptr<Weapon> c;
        std::vector<std::shared_ptr<Weapon>> throwables;
    } slot;
};

struct Firing
{
    uint32_t ammo_type;
    BulletEffectType bullet_effect;
    std::vector<uint8_t> extra_data;
    FiringMode mode;
};

struct Driving
{
    std::shared_ptr<Car> car;
    bool is_driving;
    uint32_t seat_id;
    DrivingState state;
};

class Player
{
private:
    void movePlayerToChunk(Vector2u8 new_chunk);

public:
    uint8_t index;
    ClientData client;
    GameData game;
    Inventory inventory;
    Firing firing;
    Driving driving;
    std::shared_ptr<Group> group;
    std::shared_ptr<Chunk> chunk;
    Player(uint8_t index, ClientData &&client);
    void kill();
};

#endif