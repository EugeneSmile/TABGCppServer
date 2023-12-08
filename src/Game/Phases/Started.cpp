#include "Started.h"

#include <random>
#include <cmath>

#include "Config.h"
#include "Constants.h"
#include "Enums.h"
#include "Logger.h"

Started::Started(/* args */) : current_ring_index(0)
{
    preparation_time = std::chrono::duration<float>(Config::getValue("preparation_time", 1000, "Game"));
    ring_time = std::chrono::duration<float>(Config::getValue("ring_time", 1000, "Game"));
    number_of_rings = Config::getValue("number_of_rings", 4, "Game");
    first_ring_radius = Config::getValue("first_ring_radius", 1024, "Game");
    last_ring_radius = Config::getValue("last_ring_radius", 64, "Game");

    if (number_of_rings < 2)
        number_of_rings = 2;
    prepareRings();
}

void Started::prepareRings()
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

void Started::initialize()
{
}

GameState Started::process()
{
    return GameState::Started;
}
