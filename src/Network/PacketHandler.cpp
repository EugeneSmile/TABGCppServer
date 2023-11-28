#include "PacketHandler.h"

#include "Server.h"
#include "GeneralHelper.h"
#include "Buffer.h"

void PacketHandler::sendMessageToPeer(ENetEvent *event, ClientEventCode code, Buffer &buffer, bool reliable)
{
    buffer.setClientEventCode(code);
    ENetPacket *packet = enet_packet_create(buffer, buffer.getSize(), (reliable ? ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE : 0));
    enet_peer_send(event->peer, event->channelID, packet);
    enet_packet_dispose(packet);
}

void PacketHandler::handle(ENetEvent *event)
{
    Buffer request = Buffer(event->packet->data, event->packet->dataLength);
    ClientEventCode code = request.getClientEventCode();
    if ((code != ClientEventCode::TABGPing) && (code != ClientEventCode::PlayerUpdate))
        Logger::log->debug("Handling packet with ClientEventCode {}", static_cast<uint32_t>(code));

    switch (code)
    {
    case ClientEventCode::RoomInit:
    {
        uint8_t player_id = server->players->getPlayersCount();

        Buffer reply_room_init = Buffer(8 + server->preferences->name.size() + 4);

        reply_room_init.write(static_cast<uint8_t>(ServerResponse::Accepted));
        reply_room_init.write(static_cast<uint8_t>(GameMode::BattleRoyale));
        reply_room_init.write(uint8_t(0));
        reply_room_init.write(uint8_t(player_id));
        reply_room_init.write(uint8_t(0));
        reply_room_init.write(uint8_t(0));
        reply_room_init.write<std::string>(server->preferences->name);
        reply_room_init.write(uint8_t(0));

        sendMessageToPeer(event, ClientEventCode::RoomInitRequestResponse, reply_room_init, true);

        const std::string player_name = request.read<std::string>();
        const std::string gravestone_text = request.read<std::string>();
        const uint64_t login_key = request.read<uint64_t>();
        const bool squad_host = request.read<bool>();
        const uint8_t squad_members = request.read<uint8_t>();
        const uint32_t gear_length = request.read<uint32_t>();

        Logger::log->debug("Connection of player '{}', login key '{}'", player_name, login_key);

        server->players->addPlayer(login_key, {0, player_name});
        Player &player = server->players->getPlayer(player_id);

        Buffer reply_login = Buffer(4096);

        reply_login.write(player_id);
        reply_login.write(uint8_t(0));
        reply_login.write<std::string>(player_name);
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
        reply_login.write(server->players->getPlayersCount());
        for (uint8_t i = 0; i < server->players->getPlayersCount(); ++i)
        {
            player = server->players->getPlayer(i);
            reply_login.write(i);
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

        sendMessageToPeer(event, ClientEventCode::Login, reply_login, true);
    }
    break;

    default:
        break;
    }
}
