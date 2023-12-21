#include "Flying.h"

#include <random>

#include "Logger.h"
#include "Server.h"
#include "Player.h"

Flying::Flying(/* args */)
{
    flight_time = std::chrono::duration<float>(Config::getValue("plane_time", 45, "Game"));
    randomizePlane();
}

void Flying::randomizePlane()
{
    plane.start.y = 300;
    plane.finish.y = 300;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0, M_PI * 2);

    float angle = dis(gen);
    plane.start.x = 1448 * cos(angle); // 1448 = sqrt(2) * 1024
    plane.start.z = 1448 * sin(angle); // 1448 = sqrt(2) * 1024

    Logger::log->info("Plane start angle: {}, x: {}, z: {}", angle, plane.start.x, plane.start.z);

    dis = std::uniform_real_distribution<float>(angle + M_PI_2, angle + 3 * M_PI_2);
    angle = dis(gen);
    plane.finish.x = 1448 * cos(angle); // 1448 = sqrt(2) * 1024
    plane.finish.z = 1448 * sin(angle); // 1448 = sqrt(2) * 1024

    Logger::log->info("Plane finish angle: {}, x: {}, z: {}", angle, plane.finish.x, plane.finish.z);
}

void Flying::initialize()
{
    for (std::shared_ptr<Player> &player : server->game->players->players)
        plane.players.emplace(player);

    server->network->sendBroadcast(ClientEventCode::AllWeapons);
    flying_timer.restart();
}

GameState Flying::process()
{
    if (flying_timer.passed<std::chrono::seconds>())
        Logger::log->debug("Flying time: {}", std::chrono::duration_cast<std::chrono::seconds>(flying_timer.get()).count());
    if (flying_timer.get() >= flight_time)
    {
        std::vector<ENetPeer *> peers;
        for (const std::shared_ptr<Player> &player : plane.players)
        {
            peers.push_back(player->client.peer);
            server->network->sendBroadcast(ClientEventCode::PlayerAirplaneDropped, reinterpret_cast<void *>(&player->index));
        }
        server->network->sendMulticast(peers, ClientEventCode::AllDrop);
        plane.players.clear();
        return GameState::Started;
    }
    float plane_status = flying_timer.get() / flight_time;
    server->network->sendBroadcast(ClientEventCode::PlaneUpdate, reinterpret_cast<void *>(&plane_status));
    return GameState::Flying;
}
