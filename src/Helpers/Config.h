#ifndef HELPERS_CONFIG_H_GUARD
#define HELPERS_CONFIG_H_GUARD

#include <string>
#include <cstdint>

#include <toml.hpp>

#include "TypeTraits.h"
#include "Version.h"

class Config
{
private:
    inline static const std::string config_filename = std::string(PROJECT_NAME) + ".toml";
    inline static bool loaded = false;
    inline static toml::value config;

public:
    static void init();
    static void deinit();

    template <typename T>
        requires IsInteger<T>
    static int64_t getValue(const std::string &name, T default_value, const std::string &group = "General")
    {
        if (Config::loaded)
        {
            if (!Config::config.contains(group))
                config[group] = {};
            toml::table &table = toml::find<toml::table>(Config::config, group);
            if (!table.contains(name))
                table[name] = default_value;
            return table[name].as_integer();
        }
        return default_value;
    }

    template <typename T>
        requires IsFloat<T>
    static double getValue(const std::string &name, T default_value, const std::string &group = "General")
    {
        if (Config::loaded)
        {
            if (!Config::config.contains(group))
                config[group] = {};
            toml::table &table = toml::find<toml::table>(Config::config, group);
            if (!table.contains(name))
                table[name] = default_value;
            return table[name].as_floating();
        }
        return default_value;
    }

    template <typename T>
        requires IsString<T>
    static std::string getValue(const std::string &name, T default_value, const std::string &group = "General")
    {
        if (Config::loaded)
        {
            if (!Config::config.contains(group))
                config[group] = {};
            toml::table &table = toml::find<toml::table>(Config::config, group);
            if (!table.contains(name))
                table[name] = default_value;
            return table[name].as_string();
        }
        return default_value;
    }

    template <typename T>
        requires IsBool<T>
    static bool getValue(const std::string &name, T default_value, const std::string &group = "General")
    {
        if (Config::loaded)
        {
            if (!Config::config.contains(group))
                config[group] = {};
            toml::table &table = toml::find<toml::table>(Config::config, group);
            if (!table.contains(name))
                table[name] = default_value;
            return table[name].as_boolean();
        }
        return default_value;
    }
};

#endif