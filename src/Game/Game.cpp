#include "Game.h"

#include <random>
#include <cmath>

#include <magic_enum.hpp>

#include "Config.h"
#include "Server.h"

Game::Game(/* args */)
{
    countdown_time = Config::getValue("countdown_time", 20, "Game");
    counter_countdown = Counter(std::chrono::seconds(static_cast<uint64_t>(countdown_time)));
    preparation_time = Config::getValue<float>("preparation_time", 1000, "Game");
    ring_time = Config::getValue<float>("ring_time", 1000, "Game");
    number_of_rings = Config::getValue("number_of_rings", 4, "Game");
    first_ring_radius = Config::getValue("first_ring_radius", 1024, "Game");
    last_ring_radius = Config::getValue("last_ring_radius", 64, "Game");

    if (number_of_rings < 2)
        number_of_rings = 2;
    prepareRings();
    randomizePlane();
}

void Game::changeState(GameState state)
{
    if (this->state != state)
    {
        Logger::log->info("Changing game state from {} to {}", magic_enum::enum_name(this->state), magic_enum::enum_name(state));
        this->state = state;
        server->network->packet_handler.doRequest(ClientEventCode::GameStateChanged);
    }
}

float Game::getCountdownCounter()
{
    return 0;
}

void Game::prepareRings()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_angle(0, M_PI * 2);
    std::uniform_real_distribution<float> dis_dist(0, WORLD_SIZE.x / 2 - first_ring_radius);
    for (size_t i = 0; i < number_of_rings; ++i)
    {
        Ring ring;
        ring.index = i;
        ring.radius = first_ring_radius - (first_ring_radius - last_ring_radius) * i / (number_of_rings - 1);
        float angle = dis_angle(gen);
        dis_dist = std::uniform_real_distribution<float>(0, (i == 0 ? WORLD_SIZE.x / 2 - first_ring_radius : rings[i - 1].radius - ring.radius));
        float distance = dis_dist(gen);
        ring.center.x = distance * cos(angle);
        ring.center.y = 0;
        ring.center.z = distance * sin(angle);
        ring.travelled_time = 0;
        ring.data_type = RingDataType::NextRingData;
        rings.push_back(ring);
        Logger::log->info("Ring {}: size {} at ({}, {}, {})", i, ring.radius, ring.center.x, ring.center.y, ring.center.z);
    }
}

void Game::randomizePlane()
{
    plane.start.y = 300;
    plane.finish.y = 300;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0, M_PI * 2);

    float angle = dis(gen);
    plane.start.x = sqrt(2) * 1024 * cos(angle);
    plane.start.z = sqrt(2) * 1024 * sin(angle);

    Logger::log->info("Plane start angle: {}, x: {}, z: {}", angle, plane.start.x, plane.start.z);

    dis = std::uniform_real_distribution<float>(angle - M_2_PI, angle + M_2_PI);
    angle = dis(gen);
    plane.finish.x = sqrt(2) * 1024 * cos(angle);
    plane.finish.z = sqrt(2) * 1024 * sin(angle);

    Logger::log->info("Plane finish angle: {}, x: {}, z: {}", angle, plane.finish.x, plane.finish.z);
}

void Game::stageWaiting()
{
}
void Game::stageCountdown()
{
    if (!counter_countdown.active)
        counter_countdown.start();
    if (counter_countdown.getSeconds() == std::chrono::seconds(0))
    {
        changeState(GameState::Flying);
        counter_flying.start();
    }
}
void Game::stageFlying()
{
}

void Game::stageStarted()
{
}

void Game::stageEnded()
{
}

void Game::tick()
{
    switch (state)
    {
    case GameState::WaitingForPlayers:
        stageWaiting();
        break;

    case GameState::CountDown:
        stageCountdown();
        break;

    case GameState::Flying:
        stageFlying();
        break;

    case GameState::Started:
        stageStarted();
        break;

    case GameState::Ended:
        stageEnded();
        break;

    default:
    case GameState::OpenDoors:
    case GameState::RoundOver:
    case GameState::Intermission:
    case GameState::Voting:
    case GameState::VotingOver:
        changeState(GameState::WaitingForPlayers);
        break;
    }

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