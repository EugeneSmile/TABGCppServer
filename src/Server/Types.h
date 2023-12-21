#ifndef SERVER_TYPES_H_GUARD
#define SERVER_TYPES_H_GUARD

#include <string>
#include <cstdint>
#include <vector>
#include <chrono>

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

template <typename T>
struct Vector4
{
    T x;
    T y;
    T z;
    T w;
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

typedef Vector4<float> Vector4f;
typedef Vector4<uint32_t> Vector4u;
typedef Vector4<int32_t> Vector4i;
typedef Vector4<uint8_t> Vector4u8;
typedef Vector4<uint16_t> Vector4u16;
typedef Vector4u Vector4u32;
typedef Vector4<uint64_t> Vector4u64;
typedef Vector4<int8_t> Vector4i8;
typedef Vector4<int16_t> Vector4i16;
typedef Vector4i Vector4i32;
typedef Vector4<int64_t> Vector4i64;

#endif