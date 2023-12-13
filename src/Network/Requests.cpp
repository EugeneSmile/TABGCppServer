#include "Requests.h"

#include "Server.h"
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
}

void Requests::serverShutdown(void *ctx, ENetPeer *peer)
{
    server->network->packet_handler.handleRequest(peer, ClientEventCode::ServerShutDown, nullptr, true);
}

void Requests::initRoom(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(8 + server->preferences->name.size() + 4);

    request.write(ServerResponse::Accepted);
    request.write(GameMode::BattleRoyale);
    request.write(uint8_t(0));
    request.write(reinterpret_cast<NInitRoom *>(ctx)->player_game_id);
    request.write(reinterpret_cast<NInitRoom *>(ctx)->group_game_id);
    request.write<std::string>(server->preferences->name);

    server->network->packet_handler.handleRequest(peer, ClientEventCode::RoomInitRequestResponse, &request, true);
}

void Requests::login(void *ctx, ENetPeer *peer)
{
    auto player = server->players->findPlayer(*reinterpret_cast<std::string *>(ctx));

    Buffer request = Buffer(4096);

    request.write(player->service.game_index);
    request.write(server->groups->player_group_map[player->service.game_index]);
    request.write<std::string>(player->service.name);
    if (peer != nullptr)
    {
        request.write(player->service.dev);
        request.write(player->game.position);
        request.write(player->game.rotation.y);
        request.write(!bool(player->game.health));
        request.write(bool(player->game.health < 100));
        request.write(player->game.health);
        request.write(player->game.driving.is_driving);
        if (player->game.driving.is_driving)
        {
            request.write(player->game.driving.car_id);
            request.write(player->game.driving.seat_id);
        }
        request.write(uint8_t(server->players->connected.size()));
        for (const auto &other_player : server->players->players)
        {
            request.write(other_player.service.game_index);
            request.write(server->groups->player_group_map[other_player.service.game_index]);
            request.write<std::string>(other_player.service.name);
            // weapon (?)
            request.write(uint32_t(0));
            request.write(other_player.service.gear_length);
            for (size_t i = 0; i < other_player.service.gear_length; ++i)
                request.write(other_player.service.gear[i]);
            request.write(other_player.service.dev);
            request.write(other_player.service.color);
        }

        // Weapons
        request.write(server->weapons->getNumberOfWeapons());
        for (uint32_t i = 0; i < server->weapons->getNumberOfWeapons(); ++i)
        {
            Weapon &weapon = server->weapons->getWeapon(i);
            request.write(i);
            // uniqueIdentifier (?) - uint32
            request.write(weapon.type);
            // quantity (?) - uint32
            request.write(weapon.quantity);
            // spawn
            request.write(weapon.position);
        }

        // Cars
        request.write(server->cars->cars.size());
        for (const auto &car_pair : server->cars->cars)
        {
            auto &car = car_pair.second;
            request.write(car_pair.first.game_index);
            // Currently unknown (?)
            request.write(car.type);
            request.write(car.seats);
            for (uint32_t j = 0; j < car.seats; ++j)
                request.write(j);
            request.write(uint8_t(car.parts.size()));
            for (uint8_t j = 0; j < car.parts.size(); ++j)
            {
                request.write(j);
                request.write(car.parts[j].health);
                request.write<std::string>(car.parts[j].name);
            }
        }

        request.write(server->game->daytime);
        request.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->ring_spawn_delay.count());
        request.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->ring_base_time.count());

        // (?) need to clarify
        request.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.size());
        for (size_t i = 0; i < std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.size(); ++i)
        {
            // Ring sizes
            request.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.at(i).size);
            // Ring travel time
            request.write(std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.at(i).travel_time.count());
        }

        request.write(server->preferences->lives);
        request.write(server->preferences->kills);
        request.write(server->game->state);
        if (server->game->state == GameState::CountDown)
            request.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->flight_time.count());
        if (server->game->state == GameState::Flying || server->game->state == GameState::Started)
        {
            // (?) not clear boolean
            request.write(server->game->state == GameState::Flying);
            request.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start);
            request.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish);
        }
    }
    else
    {
        request.write(player->service.gear_length);
        for (size_t i = 0; i < player->service.gear_length; ++i)
            request.write(player->service.gear.at(i));
        request.write(player->service.dev);
        request.write(player->service.color);
    }
    server->network->packet_handler.handleRequest(peer, ClientEventCode::Login, &request, true);
}

void Requests::playerUpdate(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(4096);
    // Oooh, dirty
    request.write(static_cast<float>(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())));

    // Players section

    request.write(uint8_t(server->players->connected.size()));
    for (const auto &player : server->players->players)
    {
        request.write(player.service.game_index);
        PacketContainerFlags packet_flags = (player.game.driving.is_driving ? PacketContainerFlags::All : PacketContainerFlags::PlayerPosition | PacketContainerFlags::PlayerRotation | PacketContainerFlags::PlayerDirection);
        request.write(packet_flags);
        request.write(player.game.driving.state);
        if (player.game.driving.state == DrivingState::Driving)
        {
            Car &car = server->cars->cars[player.game.driving.car_id].second;
            if (packet_flags & PacketContainerFlags::CarPosition || packet_flags == PacketContainerFlags::All)
                request.write(car.position);
            if (packet_flags & PacketContainerFlags::CarRotation || packet_flags == PacketContainerFlags::All)
            {
                request.write<Quaternion>(car.rotation);
            }
            if (packet_flags & PacketContainerFlags::CarInput || packet_flags == PacketContainerFlags::All)
                request.write(convertFloatToUInt8(car.input));
            if (packet_flags & PacketContainerFlags::PlayerRotation || packet_flags == PacketContainerFlags::All)
                request.write(player.game.rotation);
            request.write(car.driving_state);
        }
        else
        {
            if (packet_flags & PacketContainerFlags::PlayerPosition || packet_flags == PacketContainerFlags::All)
                request.write(player.game.position);
            if (player.game.driving.state != DrivingState::Slow)
            {
                if (packet_flags & PacketContainerFlags::PlayerRotation || packet_flags == PacketContainerFlags::All)
                    request.write(player.game.rotation);
                request.write(player.game.aim_down_sight);
                if (packet_flags & PacketContainerFlags::PlayerDirection || packet_flags == PacketContainerFlags::All)
                    request.write(convertFloatToUInt8(player.game.direction));
                request.write(player.game.movement_type);
            }
            else
            {
                request.write(player.game.rotation.y);
            }
        }
    }

    // Cars section

    request.write(uint8_t(server->cars->cars.size()));
    for (auto &car : server->cars->cars)
    {
        request.write(car.first.game_index);
        request.write(car.second.position);
        request.write<Quaternion>(car.second.rotation);
    }

    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerUpdate, &request, true);
}

void Requests::playerLeft(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(3);
    request.write(reinterpret_cast<NPlayerLeft *>(ctx)->player_game_id);
    request.write(reinterpret_cast<NPlayerLeft *>(ctx)->player_destroy);
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerLeft, &request, true);
}

void Requests::playerRespawn(void *ctx, ENetPeer *peer)
{
    std::vector<uint8_t> *player_ids = reinterpret_cast<std::vector<uint8_t> *>(ctx);
    Buffer request = Buffer(2 + 23 * player_ids->size());
    request.write(static_cast<uint8_t>(player_ids->size()));
    for (auto player_id : *player_ids)
    {
        auto player = server->players->findPlayer(player_id);
        request.write(player->service.game_index);
        // New index (let be the same now)
        request.write(player->service.game_index);
        request.write(player->game.health);
        request.write(player->game.position);
        request.write(player->game.rotation.x);
        request.write(player->game.curse_id);
    }
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerRespawn, &request, true);
}

void Requests::playerFire(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(31);
    uint8_t player_id = *reinterpret_cast<uint8_t *>(ctx);
    auto player = server->players->findPlayer(player_id);
    request.write(player->service.game_index);
    request.write(player->game.firing.mode);
    request.write(uint32_t(0));
    if (player->game.firing.mode & FiringMode::ContainsDirection)
    {
        request.write(player->game.position);
        // ToDo
        Quaternion quat = Quaternion(4);
        request.write<Quaternion>(quat);
    }
    // ToDo: Sync Index
    if (player->game.firing.mode & FiringMode::WantsToBeSynced)
        request.write(uint32_t(0));
    if (player->game.firing.mode & FiringMode::UseBulletEffect)
        request.write(player->game.firing.bullet_effect);
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerFire, &request, true);
}

void Requests::gameStateChanged(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(31);
    request.write(server->game->state);
    switch (server->game->state)
    {
    case GameState::CountDown:
        request.write(std::dynamic_pointer_cast<Countdown>(server->game->phases.at(GameState::CountDown))->countdown_time.count());
        break;

    case GameState::Flying:
        request.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start);
        request.write(std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish);
        request.write(server->game->daytime);
        request.write(server->game->modifier);
        break;

    case GameState::Ended:
        // ToDo: winning group
        request.write(uint8_t(0));
        break;

    default:
        break;
    }

    server->network->packet_handler.handleRequest(peer, ClientEventCode::GameStateChanged, &request, true);
}

void Requests::weaponPickup(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(15);
    request.write(*reinterpret_cast<NWeaponPickup *>(ctx));
}

void Requests::airplaneDrop(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(26);
    auto player_id = *reinterpret_cast<uint8_t *>(ctx);
    auto player = server->players->findPlayer(player_id);
    request.write(player_id);
    request.write(player->game.position);
    request.write(player->game.direction);
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerAirplaneDropped, &request, true);
}

void Requests::chatMessage(void *ctx, ENetPeer *peer)
{
    server->network->packet_handler.handleRequest(peer, ClientEventCode::ChatMessage, reinterpret_cast<Buffer *>(ctx), true);
}

void Requests::throwChatMessage(void *ctx, ENetPeer *peer)
{
    Buffer *response = reinterpret_cast<Buffer *>(ctx);
    uint8_t player_id = response->read<uint8_t>();
    std::string message = response->read<std::string>();

    auto player = server->players->findPlayer(player_id);

    Buffer request = Buffer(5 * sizeof(float) + 2 * sizeof(uint8_t) + message.size());
    request.write(player_id);
    request.write(player->game.position);
    request.write(player->game.direction);
    request.write<std::string>(message);
}

void Requests::spawnGun(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(20);
    request.write(uint32_t(10));
    request.write(uint32_t(10));
    request.write(float(0));
    request.write(float(100));
    request.write(float(0));
    server->network->packet_handler.handleRequest(peer, ClientEventCode::SpawnGun, &request, true);
}

void Requests::ringUpdate(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(19);
    Ring *ring = reinterpret_cast<Ring *>(ctx);
    request.write(static_cast<uint8_t>(ring->data_type));
    switch (ring->data_type)
    {
    default:
    case RingDataType::FlyingTime:
        request.write(ring->progress);
        break;

    case RingDataType::NextRingData:
        request.write(ring->index);
        request.write(ring->center);
        request.write(ring->size);
        break;
    }
    server->network->packet_handler.handleRequest(peer, ClientEventCode::RingUpdate, &request, true);
}

void Requests::startRegisterDamage(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(2);
    request.write(*reinterpret_cast<uint8_t *>(ctx));
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerRegMessage, &request, true);
}

void Requests::planeUpdate(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(5);
    request.write(*reinterpret_cast<float *>(ctx));
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlaneUpdate, &request, true);
}