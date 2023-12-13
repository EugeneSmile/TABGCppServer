#include "Game.h"

#include <magic_enum.hpp>

#include "Config.h"
#include "Server.h"
#include "Constants.h"

Game::Game(/* args */)
{
    phases[GameState::WaitingForPlayers] = std::make_shared<Waiting>();
    phases[GameState::CountDown] = std::make_shared<Countdown>();
    phases[GameState::Flying] = std::make_shared<Flying>();
    phases[GameState::Started] = std::make_shared<Started>();
    phases[GameState::Ended] = std::make_shared<Ended>();
    phases[GameState::OpenDoors] = std::make_shared<OpenDoors>();
}

void Game::changeState(GameState state, bool force)
{
    if (this->state != state || force)
    {
        Logger::log->info("Changing game state from {} to {}", magic_enum::enum_name(this->state), magic_enum::enum_name(state));
        this->state = state;
        server->network->packet_handler.doRequest(ClientEventCode::GameStateChanged);
    }
}

void Game::tick()
{
    changeState(phases.at(state)->tick());
    keepPlayersInBounds();
}

void Game::keepPlayersInBounds()
{
    std::vector<uint8_t> players_to_teleport;
    for (auto &player : server->players->players)
    {
        bool teleport_back = false;
        if (player.game.position.x < -PLAYER_POSITION_LIMIT.x / 2)
        {
            player.game.position.x = -PLAYER_POSITION_LIMIT.x / 2 + CHUNK_SIZE.x / 8;
            teleport_back = true;
        }
        if (player.game.position.z < -PLAYER_POSITION_LIMIT.y / 2)
        {
            player.game.position.z = -PLAYER_POSITION_LIMIT.y / 2 + CHUNK_SIZE.y / 8;
            teleport_back = true;
        }
        if (player.game.position.x > PLAYER_POSITION_LIMIT.x / 2)
        {
            player.game.position.x = PLAYER_POSITION_LIMIT.x / 2 - CHUNK_SIZE.x / 8;
            teleport_back = true;
        }
        if (player.game.position.z > PLAYER_POSITION_LIMIT.y / 2)
        {
            player.game.position.z = PLAYER_POSITION_LIMIT.y / 2 - CHUNK_SIZE.y / 8;
            teleport_back = true;
        }
        if (teleport_back)
            players_to_teleport.push_back(player.service.game_index);
    }
    if (!players_to_teleport.empty())
        server->network->packet_handler.doRequest(ClientEventCode::PlayerRespawn, static_cast<void *>(&players_to_teleport));
}