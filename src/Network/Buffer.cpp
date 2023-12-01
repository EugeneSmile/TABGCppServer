#include "Buffer.h"

Buffer::Buffer(uint8_t *data, size_t size) : mem_to_delete(false), size(size), data(data), pos(data + 1)
{
}

Buffer::Buffer(size_t size) : mem_to_delete(true), size(size), data(new uint8_t[size + 1]()), pos(data + 1)
{
}

Buffer::~Buffer()
{
    if (mem_to_delete)
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

void Buffer::finish(uint8_t byte)
{
    memset(pos, byte, data + size - pos);
}

size_t Buffer::getSize()
{
    return size;
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

std::vector<uint8_t> Buffer::getVector()
{
    return std::vector<uint8_t>(data, data + size);
}