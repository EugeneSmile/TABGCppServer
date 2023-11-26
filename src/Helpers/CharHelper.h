#ifndef HELPERS_CHAR_HELPER_H_GUARD
#define HELPERS_CHAR_HELPER_H_GUARD

#include <vector>
#include <cstdint>

template <typename T>
void pushAsChars(std::vector<uint8_t> &dest, T *value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(value))[i - 1]);
}

template <typename T>
void pushAsChars(std::vector<uint8_t> &dest, T value)
{
    for (size_t i = sizeof(T); i > 0; --i)
        dest.push_back(static_cast<char *>(static_cast<void *>(&value))[i - 1]);
}

#endif