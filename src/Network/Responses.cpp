#include "Responses.h"

#include "Server.h"
#include "Buffer.h"

Responses::Responses()
{
    function[ClientEventCode::RoomInit] = std::bind(&Responses::initRoom, this, std::placeholders::_1);
    function[ClientEventCode::PlayerUpdate] = std::bind(&Responses::playerUpdate, this, std::placeholders::_1);
    function[ClientEventCode::PlayerFire] = std::bind(&Responses::playerFire, this, std::placeholders::_1);
}

void Responses::initRoom(ENetEvent *event)
{
    Buffer request = Buffer(event->packet->data, event->packet->dataLength);
    PlayerServiceData data;

    data.name = request.read<std::string>();
    data.gravestone_text = request.read<std::string>();
    data.session_id = request.read<uint64_t>();
    data.squad_host = request.read<bool>();
    data.squad_members = request.read<uint8_t>();
    data.gear_length = request.read<uint32_t>();
    data.gear.resize(data.gear_length);
    for (size_t i = 0; i < data.gear_length; ++i)
        data.gear[i] = request.read<int32_t>();

    // This string is somehow broken in protocol
    // item = request.read<std::string>();
    request.moveForward(0x1d7);
    data.playfab_id = request.read<std::string>();
    std::string string1 = request.read<std::string, uint32_t>();
    std::string string2 = request.read<std::string, uint32_t>();
    data.color = request.read<uint32_t>();
    data.auto_squad = request.read<bool>();
    data.auth_player_id = request.read<std::string>();
    data.peer_id = event->peer->connectID;

    auto player = server->players->addPlayer(data);
    uint8_t player_game_index = player->service.game_index;
    server->network->peers.emplace(player->service.playfab_id, event->peer);

    Buffer reply_room_init = Buffer(8 + server->preferences->name.size() + 4);

    reply_room_init.write(ServerResponse::Accepted);
    reply_room_init.write(GameMode::BattleRoyale);
    reply_room_init.write(uint8_t(0));
    reply_room_init.write(uint8_t(player_game_index));
    reply_room_init.write(uint8_t(0));
    reply_room_init.write(uint8_t(0));
    reply_room_init.write<std::string>(server->preferences->name);
    reply_room_init.write(uint8_t(0));

    server->network->packet_handler.handleRequest(event->peer, ClientEventCode::RoomInitRequestResponse, reply_room_init, true);

    Logger::log->debug("Login of player '{}', login key '{}', playfab id {}, auth_id {}", player->service.name, player->service.session_id, player->service.playfab_id, player->service.auth_player_id);
    server->network->packet_handler.doRequest(ClientEventCode::Login, static_cast<void *>(&player->service.playfab_id));
}

void Responses::playerUpdate(ENetEvent *event)
{
    Buffer responce = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(responce.read<uint8_t>());
    player->game.position = responce.read<Vector3f>();
    player->game.rotation = responce.read<Vector2f>();
    player->game.aim_down_sight = responce.read<bool>();
    player->game.direction = responce.read<Vector3u8>();
    player->game.movement_type = static_cast<MovementType>(responce.read<uint8_t>());
}

void Responses::playerFire(ENetEvent *event)
{
    Buffer responce = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(responce.read<uint8_t>());
    player->game.firing.mode = static_cast<FiringMode>(responce.read<uint8_t>());
    player->game.firing.ammo_type = responce.read<uint32_t>();
    player->game.firing.extra_data.resize(responce.getRemainSize());
    for (size_t i = 0; i < responce.getRemainSize(); ++i)
        player->game.firing.extra_data[i] = responce.read<uint8_t>();
}