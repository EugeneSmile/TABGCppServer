#include "Buffer.h"

Buffer::Buffer(uint8_t *data, size_t size) : size(size), data(data), pos(data + 1)
{
    mem_to_delete = false;
}

Buffer::Buffer(size_t size) : size(size), data(new uint8_t[size]), pos(data + 1)
{
    mem_to_delete = true;
}

Buffer::~Buffer()
{
    if (mem_to_delete)
        delete data;
}

void Buffer::finish(uint8_t byte)
{
    memset(pos, byte, data + size - pos);
}

size_t Buffer::getSize()
{
    return size;
}

ClientEventCode Buffer::getClientEventCode()
{
    return static_cast<ClientEventCode>(data[0]);
}

void Buffer::setClientEventCode(ClientEventCode code)
{
    data[0] = static_cast<uint8_t>(code);
}