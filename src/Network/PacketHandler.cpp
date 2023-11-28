#include "PacketHandler.h"

#include <magic_enum.hpp>

#include "Server.h"
#include "GeneralHelper.h"
#include "Buffer.h"

PacketHandler::PacketHandler()
{
    func_responce[ClientEventCode::RoomInit] = std::bind(&PacketHandler::respInitRoom, this, std::placeholders::_1);
    func_responce[ClientEventCode::PlayerUpdate] = std::bind(&PacketHandler::respPlayerUpdate, this, std::placeholders::_1);

    func_request[ClientEventCode::Login] = std::bind(&PacketHandler::reqLogin, this, std::placeholders::_1, std::placeholders::_2);
    func_request[ClientEventCode::SpawnGun] = std::bind(&PacketHandler::reqSpawnGun, this, std::placeholders::_1, std::placeholders::_2);
}

void PacketHandler::callBroadcast(ClientEventCode code, void *ctx)
{
    for (auto &player : server->players->players)
        func_request[code](server->network->peers[player.first.key], ctx);
}

void PacketHandler::sendMessageToPeer(ENetPeer *peer, ClientEventCode code, Buffer &buffer, bool reliable)
{
    buffer.setClientEventCode(code);
    ENetPacket *packet = enet_packet_create(buffer, buffer.getSize(), (reliable ? ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE : 0));
    enet_peer_send(peer, 0, packet);
    enet_packet_dispose(packet);
}

void PacketHandler::handle(ENetEvent *event)
{
    ClientEventCode code = static_cast<ClientEventCode>(event->packet->data[0]);
    if ((code != ClientEventCode::TABGPing) && (code != ClientEventCode::PlayerUpdate))
        Logger::log->debug("Handling packet with ClientEventCode {}", magic_enum::enum_name(code));

    if (func_responce.contains(code))
        func_responce[code](event);
}

void PacketHandler::respInitRoom(ENetEvent *event)
{
    Buffer request = Buffer(event->packet->data, event->packet->dataLength);
    LoginData login_data;

    login_data.player_name = request.read<std::string>();
    login_data.gravestone_text = request.read<std::string>();
    login_data.login_key = request.read<uint64_t>();
    login_data.squad_host = request.read<bool>();
    login_data.squad_members = request.read<uint8_t>();
    login_data.gear_length = request.read<uint32_t>();

    auto player = server->players->addPlayer(event->peer->connectID, login_data);
    uint8_t player_game_index = player->first.game_index;
    server->network->peers.emplace(player->first.key, event->peer);

    Buffer reply_room_init = Buffer(8 + server->preferences->name.size() + 4);

    reply_room_init.write(static_cast<uint8_t>(ServerResponse::Accepted));
    reply_room_init.write(static_cast<uint8_t>(GameMode::BattleRoyale));
    reply_room_init.write(uint8_t(0));
    reply_room_init.write(uint8_t(player_game_index));
    reply_room_init.write(uint8_t(0));
    reply_room_init.write(uint8_t(0));
    reply_room_init.write<std::string>(server->preferences->name);
    reply_room_init.write(uint8_t(0));

    sendMessageToPeer(event->peer, ClientEventCode::RoomInitRequestResponse, reply_room_init, true);

    Logger::log->debug("Login of player '{}', login key '{}'", player->second.name, player->first.key);
    callBroadcast(ClientEventCode::Login, static_cast<void *>(&player->first.key));

    callBroadcast(ClientEventCode::SpawnGun, nullptr);
}

void PacketHandler::reqLogin(ENetPeer *peer, void *ctx)
{
    uint64_t player_key = *static_cast<uint64_t *>(ctx);
    auto player_it = server->players->findPlayer(player_key);
    auto &player_data = player_it->first;
    auto &player = player_it->second;

    Buffer reply_login = Buffer(4096);

    reply_login.write(player_data.game_index);
    reply_login.write(uint8_t(0));
    reply_login.write<std::string>(player.name);
    reply_login.write(bool(true));
    reply_login.write(player.location.x);
    reply_login.write(player.location.y);
    reply_login.write(player.location.z);
    reply_login.write(player.rotation.y);
    reply_login.write(bool(false));
    reply_login.write(bool(false));
    reply_login.write(player.health);
    reply_login.write(player.is_in_car);
    if (player.is_in_car)
    {
        reply_login.write(player.in_car_id);
        reply_login.write(player.in_car_seat);
    }
    reply_login.write(static_cast<uint8_t>(server->players->connected.size()));
    for (const auto &player_pair : server->players->players)
    {
        player = player_pair.second;
        reply_login.write(player_pair.first.game_index);
        reply_login.write(player.group);
        reply_login.write<std::string>(player.name);
        // weapon (?)
        reply_login.write(uint32_t(0));
        // array size (?)
        reply_login.write(uint32_t(0));
        // for array
        // reply_login.write(uint32_t(i));
        reply_login.write(player.dev);
        // color
        reply_login.write(uint32_t(0));
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
        reply_login.write(weapon.location.x);
        reply_login.write(weapon.location.y);
        reply_login.write(weapon.location.z);
    }

    // Cars
    reply_login.write(server->cars->getNumberOfCars());
    for (uint32_t i = 0; i < server->cars->getNumberOfCars(); ++i)
    {
        Car &car = server->cars->getCar(i);
        reply_login.write(i);
        // Currently unknown (?)
        reply_login.write(car.i2);
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
    GameState state = server->game->getState();
    reply_login.write(static_cast<uint8_t>(state));
    if (state == GameState::CountDown)
        reply_login.write(server->game->getCountdownCounter());
    if (state == GameState::Flying || state == GameState::Started)
    {
        // (?) not clear boolean
        reply_login.write(bool(true));
        reply_login.write(server->game->getBus().start.x);
        reply_login.write(server->game->getBus().start.y);
        reply_login.write(server->game->getBus().start.z);
        reply_login.write(server->game->getBus().finish.x);
        reply_login.write(server->game->getBus().finish.y);
        reply_login.write(server->game->getBus().finish.z);
    }

    // (?) size of some array of uint32
    reply_login.write(uint32_t(0));
    // for (array)
    // (?) some bool
    reply_login.write(bool(false));
    // (?) color
    reply_login.write(uint32_t(0));

    sendMessageToPeer(peer, ClientEventCode::Login, reply_login, true);
}

void PacketHandler::respPlayerUpdate(ENetEvent *event)
{
    Buffer responce = Buffer(event->packet->data, event->packet->dataLength);
    Player &player = server->players->findPlayer(responce.read<uint8_t>())->second;
    player.location.x = responce.read<float>();
    player.location.y = responce.read<float>();
    player.location.z = responce.read<float>();
    player.rotation.x = responce.read<float>();
    player.rotation.y = responce.read<float>();
    responce.read<bool>();
    responce.read<uint8_t>();
    responce.read<uint8_t>();
    responce.read<uint8_t>();
    responce.read<uint8_t>();
    // Logger::log->debug("Player {} moved to ({}, {}, {}) direction ({}, {})", player.name, player.location.x, player.location.y, player.location.z, player.rotation.x, player.rotation.y);
}

void PacketHandler::reqSpawnGun(ENetPeer *peer, void *ctx)
{
    Buffer request = Buffer(20);
    request.write(uint32_t(10));
    request.write(uint32_t(10));
    request.write(float(0));
    request.write(float(100));
    request.write(float(0));
    sendMessageToPeer(peer, ClientEventCode::SpawnGun, request, true);
}
