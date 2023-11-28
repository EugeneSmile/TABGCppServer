#include "Game.h"

#include <random>

#include "Config.h"
#include "Server.h"

Game::Game(/* args */)
{
    setBus();
}

Game::~Game()
{
}

void Game::changeState(GameState state)
{
    switch (state)
    {
    case GameState::WaitingForPlayers:
        break;

    case GameState::CountDown:
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

GameState Game::getState()
{
    return state;
}

float Game::getCountdownCounter()
{
    return 0;
}

void Game::setBus()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000);

    bus.start.x = dis(gen);
    bus.start.y = 300;
    bus.start.z = dis(gen);
    bus.finish.y = 300;
    dis = (bus.start.x < 500 ? std::uniform_int_distribution<>(500, 1000) : std::uniform_int_distribution<>(0, 500));
    bus.finish.x = dis(gen);
    dis = (bus.start.z < 500 ? std::uniform_int_distribution<>(500, 1000) : std::uniform_int_distribution<>(0, 500));
    bus.finish.z = dis(gen);
}

Bus Game::getBus()
{
    return bus;
}