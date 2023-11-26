#include "Preferences.h"

#include <filesystem>

#include "Logger.h"

Preferences::Preferences(/* args */)
{
    if (std::filesystem::exists(config_filename))
    {
        try
        {
            config = toml::parse(config_filename);
            loaded = true;
        }
        catch (const std::exception &e)
        {
            Logger::log->error(e.what());
            loaded = false;
        }
    }
    else
    {
        Logger::log->warn("Config with filename {} is missing, will use default values", config_filename);
        loaded = false;
    }
}

Preferences::~Preferences()
{
}