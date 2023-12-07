#include "Responses.h"

#include "Server.h"
#include "Buffer.h"
#include "VectorConverter.h"

Responses::Responses()
{
    function[ClientEventCode::RoomInit] = std::bind(&Responses::initRoom, this, std::placeholders::_1);
    function[ClientEventCode::RequestWorldState] = std::bind(&Responses::worldState, this, std::placeholders::_1);
    function[ClientEventCode::PlayerUpdate] = std::bind(&Responses::playerUpdate, this, std::placeholders::_1);
    function[ClientEventCode::PlayerFire] = std::bind(&Responses::playerFire, this, std::placeholders::_1);
    function[ClientEventCode::PlayerLand] = std::bind(&Responses::playerLand, this, std::placeholders::_1);
    function[ClientEventCode::RequestAirplaneDrop] = std::bind(&Responses::airplaneDrop, this, std::placeholders::_1);
    function[ClientEventCode::ChatMessage] = std::bind(&Responses::chatMessage, this, std::placeholders::_1);
    function[ClientEventCode::ThrowChatMessage] = std::bind(&Responses::throwChatMessage, this, std::placeholders::_1);
    function[ClientEventCode::TABGPing] = [](ENetEvent *) {};
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
    server->network->peers.emplace(player->service.playfab_id, event->peer);

    NInitRoom payload;
    payload.player_game_id = player->service.game_index;
    payload.group_game_id = server->groups->addPlayerToGroup(data);

    server->network->packet_handler.doRequest(ClientEventCode::RoomInitRequestResponse, reinterpret_cast<void *>(&payload), event->peer);

    server->network->packet_handler.doRequest(ClientEventCode::Login, reinterpret_cast<void *>(&player->service.playfab_id), server->network->peers[player->service.playfab_id]);

    Logger::log->debug("Login of player '{}', login key '{}', playfab id {}, game id {}", player->service.name, player->service.session_id, player->service.playfab_id, player->service.game_index);
}

void Responses::worldState(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    std::string *player_playfab_id = &server->players->findPlayer(response.read<uint8_t>())->service.playfab_id;
    server->network->packet_handler.doRequest(ClientEventCode::Login, reinterpret_cast<void *>(player_playfab_id));
}

void Responses::playerUpdate(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->game.position = response.read<Vector3f>();
    player->game.rotation = response.read<Vector2f>();
    player->game.aim_down_sight = response.read<bool>();
    player->game.direction = convertUInt8ToFloat(response.read<Vector3u8>());
    player->game.movement_type = static_cast<MovementType>(response.read<uint8_t>());
}

void Responses::playerFire(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->game.firing.mode = static_cast<FiringMode>(response.read<uint8_t>());
    player->game.firing.ammo_type = response.read<uint32_t>();
    player->game.firing.extra_data.resize(response.getRemainSize());
    for (size_t i = 0; i < response.getRemainSize(); ++i)
        player->game.firing.extra_data[i] = response.read<uint8_t>();
}

void Responses::airplaneDrop(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->game.direction = response.read<Vector3f>();
    // ToDo - find proper place for request
    server->network->packet_handler.doRequest(ClientEventCode::PlayerAirplaneDropped, reinterpret_cast<void *>(&player->service.game_index));
}

void Responses::playerLand(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->game.position = response.read<Vector3f>();
    server->network->packet_handler.doRequest(ClientEventCode::PlayerRegMessage, reinterpret_cast<void *>(&player->service.game_index));
}

void Responses::ringDeath(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->kill();
}

void Responses::playerDead(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->kill();
}

void Responses::syncProjectile(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    // ToDo
}

void Responses::curseCleanse(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    // ToDo
}

void Responses::respawnTeammate(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    // ToDo
}

void Responses::clickInteract(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    // ToDo: drop_index = response.read<uint32_t>();
}

void Responses::healthState(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    player->game.health = response.read<float>();
}

void Responses::chatMessage(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    server->network->packet_handler.doRequest(ClientEventCode::ChatMessage, &response);
}

void Responses::throwChatMessage(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    server->network->packet_handler.doRequest(ClientEventCode::ThrowChatMessage, &response);
}

void Responses::reviveState(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
}

void Responses::markerChange(ENetEvent *event)
{
    Buffer response = Buffer(event->packet->data, event->packet->dataLength);
    auto player = server->players->findPlayer(response.read<uint8_t>());
    // We receive MarkerActionType here, but it is easier to use it directly as boolean
    bool set_marker = !response.read<bool>();
    if (set_marker)
    {
        MarkerType type = static_cast<MarkerType>(response.getByCustomPosition(response.getSize() - 1));
        player->game.markers[type].position = response.read<Vector3f>();
        player->game.markers[type].normal = response.read<Vector3f>();
        player->game.markers[type].set = true;
    }
    else
    {
        player->game.markers[MarkerType::Marker].set = false;
    }
}
