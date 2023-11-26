#ifndef HELPERS_PREFERENCES_H_GUARD
#define HELPERS_PREFERENCES_H_GUARD

#include <string>

#include <toml.hpp>

#include "Version.h"

class Preferences
{
private:
    std::string config_filename = std::string(PROJECT_NAME) + ".toml";
    bool loaded = false;
    toml::value config;

public:
    Preferences(/* args */);
    ~Preferences();
};

#endif