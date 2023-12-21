#include "Responses.h"

#include "Server.h"
#include "Buffer.h"
#include "Packet.h"
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
    function[ClientEventCode::RingDeath] = std::bind(&Responses::ringDeath, this, std::placeholders::_1);
    function[ClientEventCode::PlayerDead] = std::bind(&Responses::playerDead, this, std::placeholders::_1);
    function[ClientEventCode::RequestWeaponPickUp] = std::bind(&Responses::weaponPickup, this, std::placeholders::_1);
    function[ClientEventCode::TABGPing] = [](std::unique_ptr<Packet> &) {};
}

void Responses::initRoom(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    ClientData data;

    data.name = buffer.read<std::string>();
    data.gravestone_text = buffer.read<std::string>();
    data.session_id = buffer.read<uint64_t>();
    data.group_host = buffer.read<bool>();
    data.group_size = buffer.read<uint8_t>();
    data.gear.resize(buffer.read<uint32_t>());
    for (size_t i = 0; i < data.gear.size(); ++i)
        data.gear[i] = buffer.read<int32_t>();

    // This string is somehow broken in protocol
    // item = buffer.read<std::string>();
    buffer.moveForward(0x1d7);
    data.playfab_id = std::stoull(buffer.read<std::string>(), nullptr, 16);
    std::string string1 = buffer.read<std::string, uint32_t>();
    std::string string2 = buffer.read<std::string, uint32_t>();
    data.color = buffer.read<uint32_t>();
    data.auto_squad = buffer.read<bool>();
    data.auth_player_id = buffer.read<std::string>();
    data.peer = packet->event->peer;
    data.dev = false;

    const std::shared_ptr<Player> &player = *server->game->players->addPlayer(std::move(data));

    server->network->sendUnicast(packet->event->peer, ClientEventCode::RoomInitRequestResponse, reinterpret_cast<void *>(&player->index));
    server->network->sendUnicast(packet->event->peer, ClientEventCode::Login, reinterpret_cast<void *>(&player->index));

    Logger::log->debug("Login of player '{}', playfab id {}, game id {}", player->client.name, player->client.playfab_id, player->index);
}

void Responses::worldState(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    uint8_t player_index = buffer.read<uint8_t>();
    server->network->sendBroadcast(ClientEventCode::Login, reinterpret_cast<void *>(&player_index));
}

void Responses::playerUpdate(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->game.position = buffer.read<Vector3f>();
    player->game.rotation = buffer.read<Vector2f>();
    player->game.aim_down_sight = buffer.read<bool>();
    player->game.direction = convertUInt8ToFloat(buffer.read<Vector3u8>());
    player->game.movement_type = static_cast<MovementType>(buffer.read<uint8_t>());
    server->game->chunks->playerUpdate(player);
}

void Responses::playerFire(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->firing.mode = static_cast<FiringMode>(buffer.read<uint8_t>());
    player->firing.ammo_type = buffer.read<uint32_t>();
    player->firing.extra_data.resize(buffer.getRemainSize());
    for (size_t i = 0; i < buffer.getRemainSize(); ++i)
        player->firing.extra_data[i] = buffer.read<uint8_t>();
    server->network->sendBroadcast(ClientEventCode::PlayerFire, reinterpret_cast<void *>(&player->index));
}

void Responses::airplaneDrop(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->game.direction = buffer.read<Vector3f>();
    // ToDo - find proper place for request
    // ToDo - change to chunk multicast
    server->game->getPhase<Flying>(GameState::Flying)->plane.players.erase(player);
    server->network->sendBroadcast(ClientEventCode::PlayerAirplaneDropped, reinterpret_cast<void *>(&player->index));
}

void Responses::playerLand(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->game.position = buffer.read<Vector3f>();
    server->network->sendBroadcast(ClientEventCode::PlayerLand, reinterpret_cast<void *>(&player->index));
    server->network->sendBroadcast(ClientEventCode::PlayerRegMessage, reinterpret_cast<void *>(&player->index));
}

void Responses::ringDeath(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->kill();
}

void Responses::playerDead(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->kill();
}

void Responses::syncProjectile(std::unique_ptr<Packet> &packet)
{
    // const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    //  ToDo
}

void Responses::curseCleanse(std::unique_ptr<Packet> &packet)
{
    // const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    //  ToDo
}

void Responses::respawnTeammate(std::unique_ptr<Packet> &packet)
{
    // const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    //  ToDo
}

void Responses::clickInteract(std::unique_ptr<Packet> &packet)
{
    // const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    //  ToDo: drop_index = buffer.read<uint32_t>();
}

void Responses::healthState(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    player->game.health = buffer.read<float>();
}

void Responses::chatMessage(std::unique_ptr<Packet> &packet)
{
    server->network->sendBroadcast(ClientEventCode::ChatMessage, reinterpret_cast<std::unique_ptr<Packet> *>(&packet));
}

void Responses::throwChatMessage(std::unique_ptr<Packet> &packet)
{
    server->network->sendBroadcast(ClientEventCode::ThrowChatMessage, reinterpret_cast<std::unique_ptr<Packet> *>(&packet));
}

void Responses::reviveState(std::unique_ptr<Packet> &packet)
{
    // Buffer &buffer = packet->buffer;
    //     const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
}

void Responses::markerChange(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    const std::shared_ptr<Player> &player = server->game->players->players.at(buffer.read<uint8_t>());
    // We receive MarkerActionType here, but it is easier to use it directly as boolean
    bool set_marker = !buffer.read<bool>();
    if (set_marker)
    {
        MarkerType type = static_cast<MarkerType>(buffer.getByCustomPosition(buffer.getSize() - 1));
        player->game.markers[type].position = buffer.read<Vector3f>();
        player->game.markers[type].normal = buffer.read<Vector3f>();
        player->game.markers[type].set = true;
    }
    else
    {
        player->game.markers[MarkerType::Marker].set = false;
    }
}

void Responses::weaponPickup(std::unique_ptr<Packet> &packet)
{
    Buffer &buffer = packet->buffer;
    NWeaponPickup payload;
    payload.player_index = buffer.read<uint8_t>();
    payload.weapon_index = buffer.read<uint32_t>();
    payload.weapon = server->game->weapons->weapons.at(payload.weapon_index);
    payload.slot = buffer.read<uint8_t>();
    Logger::log->info("Slot: {}", payload.slot);
    server->network->sendBroadcast(ClientEventCode::WeaponPickUpAccepted, reinterpret_cast<void *>(&payload));
}
