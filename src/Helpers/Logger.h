#ifndef HELPERS_LOGGER_H_GUARD
#define HELPERS_LOGGER_H_GUARD

#include <memory>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>

class Logger
{
private:
    inline static const size_t queue_size = 4096;
    inline static std::vector<spdlog::sink_ptr> sinks;
    inline static spdlog::sink_ptr packet_sink;

public:
    static void init();
    static void deinit();
    static void createPacketLogger();
    inline static std::shared_ptr<spdlog::async_logger> log;
    inline static std::shared_ptr<spdlog::async_logger> packet_log;
};

#endif