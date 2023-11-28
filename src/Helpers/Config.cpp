#include "Config.h"

#include <fstream>
#include <filesystem>

#include "Logger.h"

void Config::init()
{
    if (!std::filesystem::exists(Config::config_filename))
    {
        std::ofstream fs(Config::config_filename);
        fs << "\n";
        fs.close();
    }
    try
    {
        Config::config = toml::parse(Config::config_filename);
        Config::loaded = true;
    }
    catch (const std::exception &e)
    {
        Logger::log->error(e.what());
        Config::loaded = false;
    }
}

void Config::deinit()
{
    if (loaded)
    {
        std::ofstream fs(Config::config_filename);
        fs << Config::config;
        fs.close();
    }
}