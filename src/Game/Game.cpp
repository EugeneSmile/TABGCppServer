#include "Game.h"

#include <magic_enum.hpp>

#include "Config.h"
#include "Server.h"
#include "Constants.h"

Game::Game(/* args */)
{
    players = std::make_unique<Players>();
    groups = std::make_unique<Groups>();
    weapons = std::make_unique<Weapons>();
    cars = std::make_unique<Cars>();
    chunks = std::make_unique<Chunks>();

    phases[GameState::WaitingForPlayers] = std::make_shared<Waiting>();
    phases[GameState::CountDown] = std::make_shared<Countdown>();
    phases[GameState::Flying] = std::make_shared<Flying>();
    phases[GameState::Started] = std::make_shared<Started>();
    phases[GameState::Ended] = std::make_shared<Ended>();
    phases[GameState::OpenDoors] = std::make_shared<OpenDoors>();

    spawn_loot_on_start = Config::getValue("spawn_loot_on_start", false, "Game");
    player_distance_limit = Config::getValue("player_distance_limit", 1024, "Game");

    experimental_init();
}

void Game::changeState(GameState state, bool force)
{
    if (this->state != state || force)
    {
        Logger::log->info("Changing game state from {} to {}", magic_enum::enum_name(this->state), magic_enum::enum_name(state));
        this->state = state;
        server->network->sendBroadcast(ClientEventCode::GameStateChanged);
    }
}

void Game::tick()
{
    // players->findDistances();
    changeState(phases.at(state)->tick());
    keepPlayersInBounds();
}

void Game::keepPlayersInBounds()
{
    std::vector<uint8_t> players_to_teleport;
    for (std::shared_ptr<Player> player : players->players)
    {
        bool teleport_back = false;
        if (player->game.position.x < -PLAYER_POSITION_LIMIT.x / 2)
        {
            player->game.position.x = -PLAYER_POSITION_LIMIT.x / 2 + CHUNK_SIZE / 8;
            teleport_back = true;
        }
        if (player->game.position.z < -PLAYER_POSITION_LIMIT.y / 2)
        {
            player->game.position.z = -PLAYER_POSITION_LIMIT.y / 2 + CHUNK_SIZE / 8;
            teleport_back = true;
        }
        if (player->game.position.x > PLAYER_POSITION_LIMIT.x / 2)
        {
            player->game.position.x = PLAYER_POSITION_LIMIT.x / 2 - CHUNK_SIZE / 8;
            teleport_back = true;
        }
        if (player->game.position.z > PLAYER_POSITION_LIMIT.y / 2)
        {
            player->game.position.z = PLAYER_POSITION_LIMIT.y / 2 - CHUNK_SIZE / 8;
            teleport_back = true;
        }
        if (teleport_back)
            players_to_teleport.push_back(player->index);
    }
    if (!players_to_teleport.empty())
        server->network->sendBroadcast(ClientEventCode::PlayerRespawn, static_cast<void *>(&players_to_teleport));
}

void Game::experimental_init()
{

    /*     for (size_t i = 0; i < 9; i++)
        {
            createMaBoi();
        }
    */
    uint32_t weapon_id = 0;
    for (int32_t i = -WORLD_SIZE.x / 4; i < WORLD_SIZE.x / 4; i += 16)
    {
        for (uint32_t j = -WORLD_SIZE.y / 4; j < WORLD_SIZE.y / 4; j += 16)
        {
            weapons->addWeapon(weapon_id, 1, {static_cast<float>(i), 140, static_cast<float>(j)});
            weapon_id < 329 ? weapon_id++ : weapon_id = 0;
        }
    }
    /*
            game->cars->addCar(3, 0, {-15, 112, 5});
            game->cars->addCar(4, 3, {-30, 112, 5});
            game->cars->addCar(3, 7, {-45, 112, 5}); */
}