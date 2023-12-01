#ifndef SERVER_TYPES_H_GUARD
#define SERVER_TYPES_H_GUARD

#include <string>
#include <cstdint>
#include <vector>

template <typename T>
struct Vector2
{
    T x;
    T y;
};

template <typename T>
struct Vector3
{
    T x;
    T y;
    T z;
};

typedef Vector2<float> Vector2f;
typedef Vector2<uint32_t> Vector2u;
typedef Vector2<int32_t> Vector2i;
typedef Vector2<uint8_t> Vector2u8;
typedef Vector2<uint16_t> Vector2u16;
typedef Vector2u Vector2u32;
typedef Vector2<uint64_t> Vector2u64;
typedef Vector2<int8_t> Vector2i8;
typedef Vector2<int16_t> Vector2i16;
typedef Vector2i Vector2i32;
typedef Vector2<int64_t> Vector2i64;

typedef Vector3<float> Vector3f;
typedef Vector3<uint32_t> Vector3u;
typedef Vector3<int32_t> Vector3i;
typedef Vector3<uint8_t> Vector3u8;
typedef Vector3<uint16_t> Vector3u16;
typedef Vector3u Vector3u32;
typedef Vector3<uint64_t> Vector3u64;
typedef Vector3<int8_t> Vector3i8;
typedef Vector3<int16_t> Vector3i16;
typedef Vector3i Vector3i32;
typedef Vector3<int64_t> Vector3i64;

struct CarServiceData
{
    uint64_t key;
    uint8_t game_index;
};

#endif