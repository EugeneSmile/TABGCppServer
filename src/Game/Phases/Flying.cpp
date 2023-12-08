#include "Flying.h"

#include <random>

#include "Logger.h"

Flying::Flying(/* args */) : flight_time(std::chrono::duration<float>(45))
{
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
    plane.start.x = 1024 * cos(angle); // Maybe still add sqrt(2)
    plane.start.z = 1024 * sin(angle); // Maybe still add sqrt(2)

    Logger::log->info("Plane start angle: {}, x: {}, z: {}", angle, plane.start.x, plane.start.z);

    dis = std::uniform_real_distribution<float>(angle + M_PI_2, angle + 3 * M_PI_2);
    angle = dis(gen);
    plane.finish.x = 1024 * cos(angle); // Maybe still add sqrt(2)
    plane.finish.z = 1024 * sin(angle); // Maybe still add sqrt(2)

    Logger::log->info("Plane finish angle: {}, x: {}, z: {}", angle, plane.finish.x, plane.finish.z);
}

void Flying::initialize()
{
    flying_timer.restart();
}

GameState Flying::process()
{
    if (flying_timer.passed<std::chrono::seconds>())
        Logger::log->debug("Flying time: {}", std::chrono::duration_cast<std::chrono::seconds>(flying_timer.get()).count());
    if (flying_timer.get() >= flight_time)
        return GameState::Started;
    return GameState::Flying;
}
