#include "PacketHandler.h"

#include "Server.h"
#include "CharHelper.h"

void PacketHandler::sendMessageToPeer(ENetEvent *event, ClientEventCode code, const std::vector<uint8_t> &data, bool reliable)
{
    std::vector<uint8_t> payload;
    payload.push_back(static_cast<uint8_t>(code));
    payload.insert(payload.end(), data.begin(), data.end());
    ENetPacket *packet = enet_packet_create(payload.data(), payload.size(), (reliable ? ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE : 0));
    enet_peer_send(event->peer, event->channelID, packet);
    //    enet_packet_destroy(packet);
}

void PacketHandler::handle(ENetEvent *event)
{
    ClientEventCode code = static_cast<ClientEventCode>(event->packet->data[0]);
    if ((code != ClientEventCode::TABGPing) && (code != ClientEventCode::PlayerUpdate))
        std::cout << "Handling packet: " << static_cast<uint32_t>(code) << std::endl;

    switch (code)
    {
    case ClientEventCode::RoomInit:
    {
        const std::string room_name = "TABGCppServer";
        uint8_t new_index = server->players->increaseLastId();

        std::vector<uint8_t> data; //(size_t(28) + room_name.size());

        pushAsChars(data, static_cast<int32_t>(ServerResponse::Accepted));
        pushAsChars(data, static_cast<int32_t>(GameMode::BattleRoyale));
        pushAsChars(data, uint32_t(1));
        pushAsChars(data, uint32_t(new_index));
        pushAsChars(data, uint32_t(0));
        pushAsChars(data, uint32_t(1));
        data.insert(data.end(), {'C', 'u', 's', 't', 'o', 'm', 'S', 'e', 'r', 'v', 'e', 'r', '\0'});
        pushAsChars(data, uint32_t(0));

        sendMessageToPeer(event, ClientEventCode::RoomInitRequestResponse, data, true);
        std::vector<uint8_t> join_message_data;
        createJoinMessage(join_message_data, new_index, "TestPlayer");
        sendMessageToPeer(event, ClientEventCode::Login, join_message_data, true);
    }
    break;

    default:
        break;
    }
}

void PacketHandler::createJoinMessage(std::vector<uint8_t> &data, uint8_t player_index, const std::string &player_name)
{
    // player index
    data.push_back(player_index);
    // group index
    data.push_back(char(0));
    // username length
    pushAsChars(data, player_name.size());
    // username
    data.insert(data.end(), player_name.begin(), player_name.end());
    // is dev
    pushAsChars(data, uint32_t(1));

    // set up locations properly
    server->players->addPlayer({player_index, 0, player_name});
    server->players->updatePlayerLocation(player_index, {0, 200, 0});

    // x
    pushAsChars(data, float(0));
    // y
    pushAsChars(data, float(200));
    // z
    pushAsChars(data, float(0));
    // rotation
    pushAsChars(data, float(0));
    // is dead
    pushAsChars(data, uint32_t(0));
    // should update health value
    pushAsChars(data, uint32_t(0));
    // health value (not needed)
    pushAsChars(data, float(100));
    // something relating to cars? not needed
    pushAsChars(data, uint32_t(0));
    // how many players are in the lobby?
    pushAsChars(data, uint32_t(1));

    // --- OTHER PLAYERS ---

    // player index
    pushAsChars(data, uint32_t(1));
    // group index
    pushAsChars(data, uint32_t(1));
    // username length
    pushAsChars(data, player_name.size());
    // username
    data.insert(data.end(), player_name.begin(), player_name.end());
    // gun
    pushAsChars(data, int32_t(315));
    // gear data (?)
    pushAsChars(data, int32_t(0));
    // is dev
    data.push_back(1);
    // colour (?)
    pushAsChars(data, int32_t(0));

    // --- END OTHER PLAYERS ---

    // --- WEAPONS SECTION ---
    // number of weapons to spawn, just leave this empty...
    pushAsChars(data, int32_t(0));
    // --- END WEAPONS SECTION ---

    // --- CARS SECTION ---
    // number of cars to spawn, just leave this empty...
    pushAsChars(data, int32_t(0));
    // --- END CARS SECTION ---

    // time of day
    pushAsChars(data, float(0));
    // seconds before first ring
    pushAsChars(data, int32_t(1000));
    // base ring time
    pushAsChars(data, int32_t(1000));
    // something ring-related, just set to false to disable
    data.push_back(0);
    // lives
    pushAsChars(data, int32_t(2));
    // kills to win
    pushAsChars(data, uint16_t(10));
    // gamestate
    data.push_back(static_cast<uint8_t>(GameState::WaitingForPlayers));
}
