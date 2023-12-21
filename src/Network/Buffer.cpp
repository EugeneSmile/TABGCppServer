#include "Buffer.h"

Buffer::Buffer(uint8_t *data, size_t size) : is_allocated(false), size(size), data(data), pos(data + 1)
{
}

Buffer::Buffer(size_t size) : is_allocated(true), size(size), data(new uint8_t[size]()), pos(data + 1)
{
}

Buffer::Buffer(ClientEventCode code, size_t size) : is_allocated(true), size(size), data(new uint8_t[size]()), pos(data + 1)
{
    data[0] = static_cast<uint8_t>(code);
}

Buffer::~Buffer()
{
    if (is_allocated)
        delete[] data;
}

void Buffer::moveForward(size_t bytes)
{
    pos += bytes;
}
void Buffer::moveBackward(size_t bytes)
{
    pos -= bytes;
}

void Buffer::finilize(uint8_t byte)
{
    memset(pos, byte, data + size - pos);
}

size_t Buffer::getSize()
{
    return size;
}

size_t Buffer::getUsedSize()
{
    return pos - data;
}

size_t Buffer::getRemainSize()
{
    return data + size - pos;
}

ClientEventCode Buffer::getClientEventCode()
{
    return static_cast<ClientEventCode>(data[0]);
}

void Buffer::setClientEventCode(ClientEventCode code)
{
    data[0] = static_cast<uint8_t>(code);
}

uint8_t Buffer::getByCustomPosition(size_t position)
{
    return data[position];
}

uint8_t *Buffer::getData()
{
    return data;
}