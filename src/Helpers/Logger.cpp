#include "Logger.h"

#include "Version.h"

void Logger::init()
{
    // spdlog::set_async_mode(queue_size);
    spdlog::init_thread_pool(Logger::queue_size, 1);
    Logger::sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    Logger::sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(std::string(PROJECT_NAME) + ".log", 1048576 * 5, 10));
    Logger::log = std::make_shared<spdlog::async_logger>(PROJECT_NAME, begin(Logger::sinks), end(Logger::sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    Logger::log->set_pattern("[%d/%m/%Y %H:%M:%S.%e] [%^%l%$] [%n] %v");
    // logger->set_level(spdlog::get_level());
    spdlog::register_logger(Logger::log);
    spdlog::flush_every(std::chrono::seconds(10));
    Logger::log->info("Logger initialized");
}

void Logger::deinit()
{
    for (size_t i = 0; i < Logger::sinks.size(); ++i)
        sinks[i].reset();
    packet_sink.reset();
    log.reset();
    packet_log.reset();
    spdlog::shutdown();
}

void Logger::createPacketLogger()
{
    Logger::packet_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(std::string(PROJECT_NAME) + ".net.log", 1048576 * 1000, 10);
    Logger::packet_log = std::make_shared<spdlog::async_logger>("Packet", packet_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    Logger::packet_log->set_pattern("[%d/%m/%Y %H:%M:%S.%e] %v");
    spdlog::register_logger(Logger::packet_log);
}
