#ifndef HELPERS_LOGGER_H_GUARD
#define HELPERS_LOGGER_H_GUARD

#include <memory>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

class Logger
{
private:
    inline static const size_t queue_size = 4096;
    inline static std::vector<spdlog::sink_ptr> sinks;

public:
    static void init();
    inline static std::shared_ptr<spdlog::async_logger> log;
};

#endif