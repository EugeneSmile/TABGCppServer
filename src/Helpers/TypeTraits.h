#ifndef HELPERS_TYPETRAITS_H_GUARD
#define HELPERS_TYPETRAITS_H_GUARD

#include <type_traits>
#include <cstdint>
#include <string>

template <typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template <typename T>
concept IsInteger = IsAnyOf<T, std::uint64_t, std::int64_t, std::uint32_t, std::int32_t, std::uint16_t, std::int16_t, std::uint8_t, std::int8_t>;

template <typename T>
concept IsFloat = IsAnyOf<T, double, float>;

template <typename T>
concept IsString = IsAnyOf<T, std::string, const std::string, char *, const char *>;

template <typename T>
concept IsBool = IsAnyOf<T, bool>;

#endif