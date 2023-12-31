#ifndef HELPERS_TYPETRAITS_H_GUARD
#define HELPERS_TYPETRAITS_H_GUARD

#include <type_traits>
#include <cstdint>
#include <string>
#include <concepts>

#include "Types.h"
#include "NetworkTypes.h"
#include "Quaternion.h"

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

template <typename T>
concept IsVectorStruct = IsAnyOf<T, Vector3f, Vector2f, Vector2u8, Vector2u16, Vector2u32, Vector2u64, Vector2i8, Vector2i16, Vector2i32, Vector2i64, Vector3u8, Vector3u16, Vector3u32, Vector3u64, Vector3i8, Vector3i16, Vector3i32, Vector3i64>;

template <typename T>
concept IsEnum = std::is_enum<T>::value;

template <typename T>
concept IsCustomStruct = IsAnyOf<T, NWeaponPickup>;

template <typename T>
concept IsQuaternion = IsAnyOf<T, Quaternion, const Quaternion>;

template <typename T>
concept IsTime = IsAnyOf<T, std::chrono::microseconds, std::chrono::milliseconds, std::chrono::seconds, std::chrono::minutes>;

#endif