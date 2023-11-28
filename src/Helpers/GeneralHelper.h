#ifndef HELPERS_GENERALHELPER_H_GUARD
#define HELPERS_GENERALHELPER_H_GUARD

#include <vector>
#include <cstdint>
#include <string>

#include "TypeTraits.h"

template <typename T>
    requires IsInteger<T> || IsFloat<T> || IsBool<T>
void pushToBuffer(std::vector<uint8_t> &dest, T *value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(value))[i - 1]);
}

template <typename T>
    requires IsInteger<T> || IsFloat<T> || IsBool<T>
void pushToBuffer(std::vector<uint8_t> &dest, T value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(&value))[i - 1]);
}

template <typename T>
    requires IsString<T>
void pushToBuffer(std::vector<uint8_t> &dest, T *value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(value))[i - 1]);
}

template <typename T>
    requires IsString<T>
void pushToBuffer(std::vector<uint8_t> &dest, T value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(&value))[i - 1]);
}

const std::string genUUID();

#endif