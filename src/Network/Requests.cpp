#include "Requests.h"

#include "Server.h"
#include "Packet.h"
#include "Buffer.h"
#include "VectorConverter.h"

Requests::Requests()
{
    function[ClientEventCode::ServerShutDown] = std::bind(&Requests::serverShutdown, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::RoomInitRequestResponse] = std::bind(&Requests::initRoom, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::Login] = std::bind(&Requests::login, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerLeft] = std::bind(&Requests::playerLeft, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerRespawn] = std::bind(&Requests::playerRespawn, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerUpdate] = std::bind(&Requests::playerUpdate, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerFire] = std::bind(&Requests::playerFire, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::GameStateChanged] = std::bind(&Requests::gameStateChanged, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::WeaponPickUpAccepted] = std::bind(&Requests::weaponPickup, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerAirplaneDropped] = std::bind(&Requests::airplaneDrop, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::ChatMessage] = std::bind(&Requests::chatMessage, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::ThrowChatMessage] = std::bind(&Requests::throwChatMessage, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::SpawnGun] = std::bind(&Requests::spawnGun, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::RingUpdate] = std::bind(&Requests::ringUpdate, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerRegMessage] = std::bind(&Requests::startRegisterDamage, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlaneUpdate] = std::bind(&Requests::planeUpdate, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerLand] = std::bind(&Requests::playerLand, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::AllDrop] = std::bind(&Requests::planeAllDrop, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::AllWeapons] = std::bind(&Requests::allWeapons, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::ChunkEntry] = std::bind(&Requests::chunkEntry, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::ChunkExit] = std::bind(&Requests::chunkExit, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerEnteredChunk] = std::bind(&Requests::chunkEntryOtherPlayer, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerLeftChunk] = std::bind(&Requests::chunkExitOtherPlayer, this, std::placeholders::_1, std::placeholders::_2);

    buffer_size[ClientEventCode::ServerShutDown] = 1;
    buffer_size[ClientEventCode::RoomInitRequestResponse] = 12 + server->preferences->name.size();
    buffer_size[ClientEventCode::Login] = 4096;
    buffer_size[ClientEventCode::PlayerLeft] = 3;
    buffer_size[ClientEventCode::PlayerRespawn] = 6144;
    buffer_size[ClientEventCode::PlayerUpdate] = 4096;
    buffer_size[ClientEventCode::PlayerFire] = 31;
    buffer_size[ClientEventCode::GameStateChanged] = 31;
    buffer_size[ClientEventCode::WeaponPickUpAccepted] = 15;
    buffer_size[ClientEventCode::PlayerAirplaneDropped] = 26;
    buffer_size[ClientEventCode::ThrowChatMessage] = 288;
    buffer_size[ClientEventCode::SpawnGun] = 20;
    buffer_size[ClientEventCode::RingUpdate] = 19;
    buffer_size[ClientEventCode::PlayerRegMessage] = 2;
    buffer_size[ClientEventCode::PlaneUpdate] = 5;
    buffer_size[ClientEventCode::PlayerLand] = 14;
    buffer_size[ClientEventCode::AllDrop] = 13;
    buffer_size[ClientEventCode::AllWeapons] = 5 + 24 * server->game->weapons->weapons.size();
    buffer_size[ClientEventCode::ChunkEntry] = 4096;
    buffer_size[ClientEventCode::ChunkExit] = 4096;
    buffer_size[ClientEventCode::PlayerEnteredChunk] = 1024;
    buffer_size[ClientEventCode::PlayerLeftChunk] = 1024;
}

void Requests::serverShutdown(std::unique_ptr<Packet> &packet, void *ctx)
{
}

void Requests::initRoom(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));

    buffer.write(ServerResponse::Accepted);
    buffer.write(GameMode::BattleRoyale);
    buffer.write(uint8_t(0));
    buffer.write(player->index);
    buffer.write(player->group->index);
    buffer.write<std::string>(server->preferences->name);
}

void Requests::login(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));

    buffer.write(player->index);
    buffer.write(player->group->index);
    buffer.write<std::string>(player->client.name);
    if (packet->type == PacketType::UNICAST)
    {
        buffer.write(player->client.dev);
        buffer.write(player->game.position);
        buffer.write(player->game.rotation.y);
        buffer.write(!bool(player->game.health));
        buffer.write(bool(player->game.health < 100));
        buffer.write(player->game.health);
        buffer.write(player->driving.is_driving);
        if (player->driving.is_driving)
        {
            buffer.write(player->driving.car->index);
            buffer.write(player->driving.seat_id);
        }

        // Players
        buffer.write(uint8_t(server->game->players->players.size()));
        for (const std::shared_ptr<Player> &other_player : server->game->players->players)
        {
            buffer.write(other_player->index);
            buffer.write(other_player->group->index);
            buffer.write<std::string>(other_player->client.name);
            // weapon (?)
            buffer.write(uint32_t(0));
            buffer.write(uint32_t(other_player->client.gear.size()));
            for (size_t i = 0; i < other_player->client.gear.size(); ++i)
                buffer.write(other_player->client.gear[i]);
            buffer.write(other_player->client.dev);
            buffer.write(other_player->client.color);
        }

        // Weapons
        if (server->game->spawn_loot_on_start)
        {
            buffer.write(uint32_t(server->game->weapons->weapons.size()));
            for (const std::shared_ptr<Weapon> &weapon : server->game->weapons->weapons)
            {
                buffer.write(weapon->index);
                buffer.write(weapon->id);
                buffer.write(weapon->quantity);
                buffer.write(weapon->position);
            }
        }
        else
        {
            buffer.write(uint32_t(0));
        }

        // Cars
        buffer.write(uint32_t(server->game->cars->cars.size()));
        for (const std::shared_ptr<Car> &car : server->game->cars->cars)
        {
            buffer.write(car->id);
            buffer.write(car->index);
            buffer.write(uint32_t(car->seats.size()));
            for (uint32_t j = 0; j < car->seats.size(); ++j)
                buffer.write(j);
            buffer.write(uint8_t(car->parts.size()));
            for (uint8_t j = 0; j < car->parts.size(); ++j)
            {
                buffer.write(j);
                buffer.write(car->parts[j].health);
                buffer.write<std::string, uint8_t>(car->parts[j].name);
            }
        }

        buffer.write(server->game->daytime);
        buffer.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->ring_spawn_delay.count());
        buffer.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->ring_base_time.count());

        // (?) need to clarify
        buffer.write(uint8_t(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.size()));
        for (size_t i = 0; i < std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.size(); ++i)
        {
            // Ring sizes
            buffer.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.at(i).size);
            // Ring travel time
            buffer.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.at(i).travel_time.count());
        }

        buffer.write(server->preferences->lives);
        buffer.write(server->preferences->kills);
        buffer.write(server->game->state);
        if (server->game->state == GameState::CountDown)
            buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->flight_time.count());
        if (server->game->state == GameState::Flying || server->game->state == GameState::Started)
        {
            // (?) not clear boolean
            buffer.write(server->game->state == GameState::Flying);
            buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start);
            buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish);
        }
    }
    buffer.write(uint32_t(player->client.gear.size()));
    for (size_t i = 0; i < player->client.gear.size(); ++i)
        buffer.write(player->client.gear.at(i));
    buffer.write(player->client.dev);
    buffer.write(player->client.color);
}

void Requests::playerUpdate(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    // Oooh, dirty
    buffer.write(static_cast<float>(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())));

    // Players section

    buffer.write(uint8_t(server->game->players->players.size()));
    for (const std::shared_ptr<Player> &player : server->game->players->players)
    {
        buffer.write(player->index);
        PacketContainerFlags packet_flags = (player->driving.is_driving ? PacketContainerFlags::All : PacketContainerFlags::PlayerPosition | PacketContainerFlags::PlayerRotation | PacketContainerFlags::PlayerDirection);
        buffer.write(packet_flags);
        buffer.write(player->driving.state);
        if (player->driving.state == DrivingState::Driving && player->driving.car)
        {
            auto &car = player->driving.car;
            if (packet_flags & PacketContainerFlags::CarPosition || packet_flags == PacketContainerFlags::All)
                buffer.write(car->position);
            if (packet_flags & PacketContainerFlags::CarRotation || packet_flags == PacketContainerFlags::All)
            {
                buffer.write<Quaternion>(car->rotation);
            }
            if (packet_flags & PacketContainerFlags::CarInput || packet_flags == PacketContainerFlags::All)
                buffer.write(convertFloatToUInt8(car->input));
            if (packet_flags & PacketContainerFlags::PlayerRotation || packet_flags == PacketContainerFlags::All)
                buffer.write(player->game.rotation);
            buffer.write(car->driving_state);
        }
        else
        {
            if (packet_flags & PacketContainerFlags::PlayerPosition || packet_flags == PacketContainerFlags::All)
                buffer.write(player->game.position);
            if (player->driving.state != DrivingState::Slow)
            {
                if (packet_flags & PacketContainerFlags::PlayerRotation || packet_flags == PacketContainerFlags::All)
                    buffer.write(player->game.rotation);
                buffer.write(player->game.aim_down_sight);
                if (packet_flags & PacketContainerFlags::PlayerDirection || packet_flags == PacketContainerFlags::All)
                    buffer.write(convertFloatToUInt8(player->game.direction));
                buffer.write(player->game.movement_type);
            }
            else
            {
                buffer.write(player->game.rotation.y);
            }
        }
    }

    // Cars section

    buffer.write(uint8_t(server->game->cars->cars.size()));
    for (const auto &car : server->game->cars->cars)
    {
        buffer.write(car->index);
        buffer.write(car->position);
        buffer.write<Quaternion>(car->rotation);
    }
}

void Requests::playerLeft(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(*reinterpret_cast<uint8_t *>(ctx));
    buffer.write(bool(true));
}

void Requests::playerRespawn(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    std::vector<uint8_t> *player_ids = reinterpret_cast<std::vector<uint8_t> *>(ctx);

    buffer.write(static_cast<uint8_t>(player_ids->size()));
    for (auto player_id : *player_ids)
    {
        const std::shared_ptr<Player> &player = server->game->players->players.at(player_id);
        buffer.write(player->index);
        // New index (let be the same now)
        buffer.write(player->index);
        buffer.write(player->game.health);
        buffer.write(player->game.position);
        buffer.write(player->game.rotation.x);
        buffer.write(player->game.curse_id);
    }
}

void Requests::playerFire(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));
    buffer.write(player->index);
    buffer.write(player->firing.mode);
    buffer.write(uint32_t(0));
    if (player->firing.mode & FiringMode::ContainsDirection)
    {
        buffer.write(player->game.position);
        // ToDo
        Quaternion quat = Quaternion(4);
        buffer.write<Quaternion>(quat);
    }
    // ToDo: Sync Index
    if (player->firing.mode & FiringMode::WantsToBeSynced)
        buffer.write(uint32_t(0));
    if (player->firing.mode & FiringMode::UseBulletEffect)
        buffer.write(player->firing.bullet_effect);
}

void Requests::gameStateChanged(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(server->game->state);
    switch (server->game->state)
    {
    case GameState::CountDown:
        buffer.write(std::dynamic_pointer_cast<Countdown>(server->game->phases.at(GameState::CountDown))->countdown_time.count());
        break;

    case GameState::Flying:
        buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start);
        buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish);
        buffer.write(server->game->daytime);
        buffer.write(server->game->modifier);
        break;

    case GameState::Ended:
        // ToDo: winning group
        buffer.write(uint8_t(0));
        break;

    default:
        break;
    }
}

void Requests::weaponPickup(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    NWeaponPickup *payload = reinterpret_cast<NWeaponPickup *>(ctx);
    buffer.write(payload->player_index);
    buffer.write(payload->weapon_index);
    buffer.write(payload->weapon->type);
    buffer.write(payload->weapon->quantity);
    buffer.write(payload->slot);
}

void Requests::airplaneDrop(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));
    buffer.write(player->index);
    buffer.write(player->game.position);
    buffer.write(player->game.direction);
}

void Requests::chatMessage(std::unique_ptr<Packet> &packet, void *ctx)
{
}

void Requests::throwChatMessage(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    // ToDo
    Buffer *response = reinterpret_cast<Buffer *>(ctx);
    uint8_t player_id = response->read<uint8_t>();
    std::string message = response->read<std::string>();

    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));

    buffer.write(player_id);
    buffer.write(player->game.position);
    buffer.write(player->game.direction);
    buffer.write<std::string>(message);
}

void Requests::spawnGun(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(uint32_t(10));
    buffer.write(uint32_t(10));
    buffer.write(float(0));
    buffer.write(float(100));
    buffer.write(float(0));
}

void Requests::ringUpdate(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    Ring *ring = reinterpret_cast<Ring *>(ctx);
    buffer.write(static_cast<uint8_t>(ring->data_type));
    switch (ring->data_type)
    {
    default:
    case RingDataType::FlyingTime:
        buffer.write(ring->progress);
        break;

    case RingDataType::NextRingData:
        buffer.write(ring->index);
        buffer.write(ring->center);
        buffer.write(ring->size);
        break;
    }
}

void Requests::startRegisterDamage(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(*reinterpret_cast<uint8_t *>(ctx));
}

void Requests::planeUpdate(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(*reinterpret_cast<float *>(ctx));
}

void Requests::playerLand(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    uint8_t player_id = *reinterpret_cast<uint8_t *>(ctx);
    buffer.write(player_id);
    buffer.write(server->game->players->players.at(player_id)->game.position);
}

void Requests::planeAllDrop(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish);
}

void Requests::allWeapons(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    buffer.write(uint32_t(server->game->weapons->weapons.size()));
    for (const std::shared_ptr<Weapon> &weapon : server->game->weapons->weapons)
    {
        buffer.write(weapon->index);
        buffer.write(weapon->id);
        buffer.write(weapon->quantity);
        buffer.write(weapon->position);
    }
}

void Requests::chunkEntry(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));

    buffer.write(player->index);
    buffer.write(uint8_t(player->chunk->players.size()));
    buffer.write(uint16_t(player->chunk->weapons.size()));
    buffer.write(uint16_t(player->chunk->cars.size()));
    for (const std::shared_ptr<Player> &other_player : player->chunk->players)
    {
        buffer.write(other_player->index);
        buffer.write(other_player->game.state);
        buffer.write(uint32_t(0));
        buffer.write(other_player->game.health);
        buffer.write(bool(other_player->game.health < 100));
        buffer.write(other_player->game.flying);
        buffer.write(uint32_t(other_player->client.gear.size()));
        for (uint32_t i = 0; i < other_player->client.gear.size(); ++i)
            buffer.write(other_player->client.gear.at(i));
        buffer.write(uint8_t(0)); // uint8_t inventory array size
        // uint16_t inventory array
        buffer.write(uint8_t(0)); // uint8_t attachments array size
                                  // uint16_t attachments array
    }
    for (const std::shared_ptr<Weapon> &weapon : player->chunk->weapons)
    {
        buffer.write(weapon->index);
        buffer.write(weapon->id);
        buffer.write(weapon->quantity);
        buffer.write(weapon->position);
    }
    for (const std::shared_ptr<Car> &car : player->chunk->cars)
    {
        buffer.write(car->index);
        // Health (?) - looks like unused in client
        buffer.write(float(100));
        buffer.write(car->position);
        buffer.write<Quaternion>(car->rotation);
        for (const std::shared_ptr<Player> &seating_player : car->seats)
        {
            if (seating_player)
                buffer.write(seating_player->index);
            else
                buffer.write(UINT8_MAX);
        }
    }
}

void Requests::chunkExit(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(*reinterpret_cast<uint8_t *>(ctx));

    buffer.write(player->index);
    buffer.write(uint8_t(player->chunk->players.size()));
    buffer.write(uint16_t(player->chunk->weapons.size()));
    buffer.write(uint16_t(player->chunk->cars.size()));
    for (const std::shared_ptr<Player> &other_player : player->chunk->players)
        buffer.write(other_player->index);
    for (const std::shared_ptr<Weapon> &weapon : player->chunk->weapons)
        buffer.write(weapon->index);
    for (const std::shared_ptr<Car> &car : player->chunk->cars)
        buffer.write(car->index);
}

void Requests::chunkEntryOtherPlayer(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    Player *player = reinterpret_cast<Player *>(ctx);
    buffer.write(player->index);
    buffer.write(player->game.state);
    // Weapon data
    buffer.write(uint32_t(0));
    buffer.write(player->game.health);
    buffer.write(player->game.knocked);
    buffer.write(player->game.flying);
    buffer.write(uint32_t(player->client.gear.size()));
    for (uint32_t i = 0; i < player->client.gear.size(); ++i)
        buffer.write(player->client.gear.at(i));
    buffer.write(uint8_t(0)); // uint8_t inventory array size
    // uint16_t inventory array
    buffer.write(uint8_t(0)); // uint8_t attachments array size
    // uint16_t attachments array
    buffer.write(player->inventory.blessings.at(0)->type);
    buffer.write(player->inventory.blessings.at(1)->type);
    buffer.write(player->inventory.blessings.at(2)->type);
    buffer.write(player->driving.state);
    if (player->driving.state == DrivingState::Driving)
    {
        buffer.write(player->driving.car->index);
        for (auto &seat : player->driving.car->seats)
            if (seat)
                buffer.write(seat->index);
            else
                buffer.write(UINT8_MAX);
    }
}

void Requests::chunkExitOtherPlayer(std::unique_ptr<Packet> &packet, void *ctx)
{
    Buffer &buffer = packet->buffer;
    Player *player = reinterpret_cast<Player *>(ctx);
    buffer.write(player->index);
    buffer.write(uint8_t(0));
}
