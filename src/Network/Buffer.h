#ifndef NETWORK_BUFFER_H_GUARD
#define NETWORK_BUFFER_H_GUARD

#include <string>
#include <cstring>
#include <cstdint>
#include <memory>
#include <vector>

#include "TypeTraits.h"
#include "Enums.h"

class Buffer
{
private:
    bool mem_to_delete;
    size_t size;
    uint8_t *data;
    uint8_t *pos;

public:
    Buffer(uint8_t *data, size_t size);
    Buffer(size_t size = 256);
    ~Buffer();
    void moveForward(size_t bytes);
    void moveBackward(size_t bytes);
    void finish(uint8_t byte = uint8_t(0));
    size_t getSize();
    size_t getRemainSize();
    ClientEventCode getClientEventCode();
    void setClientEventCode(ClientEventCode code);
    std::vector<uint8_t> getVector();

    operator uint8_t *() const
    {
        return data;
    };

    template <typename T>
        requires IsInteger<T> || IsBool<T> || IsFloat<T> || IsVectorStruct<T>
    T read()
    {
        T *ret;
        ret = static_cast<T *>(static_cast<void *>(pos));
        pos += sizeof(T);
        return *ret;
    }

    template <typename T, typename U = uint8_t>
        requires IsString<T>
    std::string read()
    {
        U length = *static_cast<U *>(static_cast<void *>(pos));
        pos += sizeof(U);
        std::string ret = std::string(static_cast<char *>(static_cast<void *>(pos)), length);
        pos += length;
        return ret;
    }

    template <typename T>
        requires IsInteger<T> || IsBool<T> || IsFloat<T> || IsVectorStruct<T>
    void write(T data)
    {
        memcpy(pos, &data, sizeof(data));
        pos += sizeof(data);
    }

    template <typename T>
        requires IsEnum<T>
    void write(T data)
    {
        write(static_cast<uint8_t>(data));
    }

    template <typename T>
        requires IsString<T>
    void write(const std::string &data)
    {
        uint32_t length = data.size();
        memcpy(pos, &length, sizeof(length));
        pos += sizeof(length);
        memcpy(pos, data.c_str(), length);
        pos += length;
    }

    template <typename T>
        requires IsString<T>
    void write(const char *data)
    {
        uint32_t length = strlen(data);
        memcpy(pos, &length, sizeof(length));
        pos += sizeof(length);
        memcpy(pos, data, length);
        pos += length;
    }
};

#endif