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

void Game::tick()
{
    switch (state)
    {
    case GameState::WaitingForPlayers:
        //        if (server->players->connected.size() > 1)
        //            changeState(GameState::CountDown);
        break;

    case GameState::CountDown:
        if (!counter_countdown.active)
            counter_countdown.start();
        if (counter_countdown.getSeconds() == std::chrono::seconds(0))
        {
            changeState(GameState::Flying);
            counter_flying.start();
        }
        break;

    case GameState::Flying:
        break;

    case GameState::Started:
        break;

    case GameState::Ended:
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
}