#include "Started.h"

#include <random>
#include <cmath>

#include "Config.h"
#include "Constants.h"
#include "Enums.h"
#include "Logger.h"
#include "Server.h"

Started::Started(/* args */) : current_ring_index(0)
{
    ring_spawn_delay = std::chrono::duration<float>(Config::getValue("ring_spawn_delay", 70, "Game"));
    ring_base_time = std::chrono::duration<float>(Config::getValue("ring_base_time", 200, "Game"));
    number_of_rings = Config::getValue("number_of_rings", 6, "Game");
    first_ring_size = Config::getValue("first_ring_size", 4240, "Game");
    last_ring_size = Config::getValue("last_ring_size", 140, "Game");

    if (number_of_rings < 2)
        number_of_rings = 2;
    prepareRings();
}

void Started::prepareRings()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_angle(0, M_PI * 2);
    std::uniform_real_distribution<float> dis_dist(0, WORLD_SIZE.x / 2 - first_ring_size);
    for (size_t i = 0; i < number_of_rings; ++i)
    {
        Ring ring;
        ring.index = i;
        ring.size = first_ring_size - (first_ring_size - last_ring_size) * i / (number_of_rings - 1);
        float angle = dis_angle(gen);
        dis_dist = std::uniform_real_distribution<float>(0, (i == 0 ? WORLD_SIZE.x / 2 - first_ring_size : rings[i - 1].size - ring.size));
        float distance = dis_dist(gen);
        ring.center.x = distance * cos(angle);
        ring.center.y = 0;
        ring.center.z = distance * sin(angle);
        // Possibly do something here
        ring.travel_time = ring_base_time;
        ring.progress = 0;
        ring.data_type = RingDataType::NextRingData;
        rings.push_back(ring);
        Logger::log->info("Ring {}: size {} at ({}, {}, {})", i, ring.size, ring.center.x, ring.center.y, ring.center.z);
    }
}

void Started::initialize()
{
    current_ring_index = 0;
    current_ring = &rings.at(current_ring_index);
    current_ring->data_type = RingDataType::NextRingData;
    server->network->sendBroadcast(ClientEventCode::RingUpdate, current_ring);
    ring_timer.restart();
}

GameState Started::process()
{
    current_ring->progress = 100 * ring_timer.get() / current_ring->travel_time;
    if (ring_timer.passed<std::chrono::seconds>())
        Logger::log->debug("Rings phase: Ring {}, progress {}", current_ring_index, current_ring->progress);
    /*
        if (ring_timer.get() >= current_ring->travel_time)
        {
            if (current_ring_index == number_of_rings - 1)
            {

                // return GameState::Ended;
            }
            else
            {
                current_ring_index++;
                current_ring = &rings.at(current_ring_index);
                current_ring->data_type = RingDataType::NextRingData;
                server->network->packet_handler.doRequest(ClientEventCode::RingUpdate, current_ring);
                current_ring->data_type = RingDataType::StartMove;
                server->network->packet_handler.doRequest(ClientEventCode::RingUpdate, current_ring);
                ring_timer.restart();
            }
        }
        else
        {
            current_ring->data_type = RingDataType::FlyingTime;
            server->network->packet_handler.doRequest(ClientEventCode::RingUpdate, current_ring);
        }*/
    return GameState::Started;
}
