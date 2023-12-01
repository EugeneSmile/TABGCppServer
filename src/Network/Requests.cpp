#include "Requests.h"

#include "Server.h"
#include "Buffer.h"

Requests::Requests()
{
    function[ClientEventCode::Login] = std::bind(&Requests::login, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerLeft] = std::bind(&Requests::playerLeft, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::PlayerUpdate] = std::bind(&Requests::playerUpdate, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::SpawnGun] = std::bind(&Requests::spawnGun, this, std::placeholders::_1, std::placeholders::_2);
    function[ClientEventCode::GameStateChanged] = std::bind(&Requests::gameStateChanged, this, std::placeholders::_1, std::placeholders::_2);
}

void Requests::login(void *ctx, ENetPeer *peer)
{
    std::string player_id = *static_cast<std::string *>(ctx);
    auto player = server->players->findPlayer(player_id);

    Buffer reply_login = Buffer(4096);

    reply_login.write(player->service.game_index);
    reply_login.write(uint8_t(0));
    reply_login.write<std::string>(player->service.name);
    reply_login.write(bool(true));
    reply_login.write(player->game.position);
    reply_login.write(player->game.rotation.y);
    reply_login.write(bool(false));
    reply_login.write(bool(false));
    reply_login.write(player->game.health);
    reply_login.write(player->game.driving.is_driving);
    if (player->game.driving.is_driving)
    {
        reply_login.write(player->game.driving.car_id);
        reply_login.write(player->game.driving.seat_id);
    }
    reply_login.write(uint8_t(server->players->connected.size()));
    for (const auto &other_player : server->players->players)
    {
        reply_login.write(other_player.service.game_index);
        reply_login.write(other_player.service.group_index);
        reply_login.write<std::string>(other_player.service.name);
        // weapon (?)
        reply_login.write(uint32_t(0));
        // array size (?)
        reply_login.write(other_player.service.gear_length);
        for (size_t i = 0; i < other_player.service.gear_length; ++i)
            reply_login.write(other_player.service.gear[i]);
        reply_login.write(other_player.service.dev);
        // color
        reply_login.write(other_player.service.color);
    }

    // Weapons
    reply_login.write(server->weapons->getNumberOfWeapons());
    for (uint32_t i = 0; i < server->weapons->getNumberOfWeapons(); ++i)
    {
        Weapon &weapon = server->weapons->getWeapon(i);
        reply_login.write(i);
        // uniqueIdentifier (?) - uint32
        reply_login.write(weapon.type);
        // quantity (?) - uint32
        reply_login.write(weapon.count);
        // spawn
        reply_login.write(weapon.location);
    }

    // Cars
    reply_login.write(server->cars->cars.size());
    for (const auto &car_pair : server->cars->cars)
    {
        auto &car = car_pair.second;
        reply_login.write(car_pair.first.game_index);
        // Currently unknown (?)
        reply_login.write(car.type);
        reply_login.write(car.seats);
        for (uint32_t j = 0; j < car.seats; ++j)
            reply_login.write(j);
        reply_login.write(uint8_t(car.parts.size()));
        for (uint8_t j = 0; j < car.parts.size(); ++j)
        {
            reply_login.write(j);
            reply_login.write(car.parts[j].health);
            reply_login.write<std::string>(car.parts[j].name);
        }
    }

    // time of day
    reply_login.write(float(0));
    // seconds to first ring
    reply_login.write(float(1000));
    // base ring time
    reply_login.write(float(1000));

    // (?) need to clarify
    uint8_t some_index = 0;
    reply_login.write(some_index);
    for (size_t i = 0; i < some_index; ++i)
    {
        reply_login.write(float(10));
        reply_login.write(float(20));
    }

    reply_login.write(server->preferences->lives);
    reply_login.write(server->preferences->kills);
    // Need to implement game state
    reply_login.write(server->game->state);
    if (server->game->state == GameState::CountDown)
        reply_login.write(server->game->getCountdownCounter());
    if (server->game->state == GameState::Flying || server->game->state == GameState::Started)
    {
        // (?) not clear boolean
        reply_login.write(bool(true));
        reply_login.write(server->game->bus.start);
        reply_login.write(server->game->bus.finish);
    }

    // (?) size of some array of uint32
    reply_login.write(uint32_t(0));
    // for (array)
    // (?) some bool
    reply_login.write(bool(false));
    // (?) color
    reply_login.write(uint32_t(0));

    server->network->packet_handler.handleRequest(peer, ClientEventCode::Login, reply_login, true);
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
                request.write(car.rotation_state);
                if (car.rotation_state < 4)
                    request.write(car.rotation);
            }
            if (packet_flags & PacketContainerFlags::CarInput || packet_flags == PacketContainerFlags::All)
                request.write(car.input);
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
                    request.write(player.game.direction);
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
    for (const auto &car : server->cars->cars)
    {
        request.write(car.first.game_index);
        request.write(car.second.position);
        request.write(car.second.rotation_state);
        if (car.second.rotation_state < 4)
            request.write(car.second.rotation);
    }

    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerUpdate, request, true);
}

void Requests::playerLeft(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(3);
    request.write(static_cast<uint8_t *>(ctx)[0]);
    request.write(static_cast<bool *>(ctx)[1]);
    server->network->packet_handler.handleRequest(peer, ClientEventCode::PlayerLeft, request, true);
}

void Requests::gameStateChanged(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(31);
    request.write(server->game->state);
    switch (server->game->state)
    {
    case GameState::CountDown:
        request.write(server->game->countdown_time);
        break;

    case GameState::Flying:
        request.write(server->game->bus.start);
        request.write(server->game->bus.finish);
        request.write(server->game->daytime);
        request.write(server->game->modifier);
        break;

    case GameState::Ended:
        // ToDo: winning group
        request.write(*static_cast<uint8_t *>(ctx));
        break;

    default:
        break;
    }

    server->network->packet_handler.handleRequest(peer, ClientEventCode::GameStateChanged, request, true);
}

void Requests::spawnGun(void *ctx, ENetPeer *peer)
{
    Buffer request = Buffer(20);
    request.write(uint32_t(10));
    request.write(uint32_t(10));
    request.write(float(0));
    request.write(float(100));
    request.write(float(0));
    server->network->packet_handler.handleRequest(peer, ClientEventCode::SpawnGun, request, true);
}
