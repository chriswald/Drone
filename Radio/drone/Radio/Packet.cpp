#include "Packet.h"

Packet::Packet()
{
    memset(payload, 0, PAYLOAD_SIZE);
}

Packet::~Packet() {}

void Packet::setMethod(PacketMethodType method)
{
    header &= 0x9F;
    header |= ((uint8_t)method & 0x03) << 5;
    setParity();
}

void Packet::setAction(PacketActionType action)
{
    header &= 0xE0;
    header |= ((uint8_t)action & 0x1F);
    setParity();
}

void Packet::setError(PacketErrorType error)
{
    Packet::error = (uint8_t)error;
    setParity();
}

void Packet::setDataArray(void* data, uint8_t size, uint8_t offset)
{
    memcpy(&(payload[offset]), data, size);
    setParity();
}

PacketMethodType Packet::getMethod()
{
    return (PacketMethodType)((header & 0x60) >> 5);
}

PacketActionType Packet::getAction()
{
    return (PacketActionType)(header & 0x1F);
}

PacketErrorType Packet::getError()
{
    return (PacketErrorType)error;
}

void Packet::getDataArray(void* data, uint8_t size, uint8_t offset)
{
    memcpy(data, &(payload[offset]), size);
}

void Packet::package(uint8_t* bytes, uint8_t* size)
{
    memcpy(bytes, &header, 1);
    memcpy(&(bytes[1]), &error, 1);
    memcpy(&(bytes[2]), payload, PAYLOAD_SIZE);
    if (size != NULL)
        *size = PACKET_SIZE;
}

void Packet::unpackage(uint8_t* bytes, uint8_t size)
{
    memcpy(&header, bytes, 1);
    memcpy(&error, &(bytes[1]), 1);
    memcpy(payload, &(bytes[2]), size-2);
}

bool Packet::isCorrupt()
{
    uint8_t parity = packetParity();
    return (parity != ((header & 0x80) >> 7));
}

void Packet::setParity()
{
    uint8_t parity = packetParity();
    header &= 0x7F;
    header |= (parity & 0x01) << 7;
}

uint8_t Packet::packetParity()
{
    return headerParity() ^ errorParity() ^ payloadParity();
}

uint8_t Packet::headerParity()
{
    uint8_t parity = 0;
    for (int i = 0; i < 7; i ++)
    {
        parity ^= (header >> i) & 0x01;
    }

    return parity;
}

uint8_t Packet::errorParity()
{
    uint8_t parity = 0;
    for (int i = 0; i < 8; i ++)
    {
        parity ^= (error >> i) & 0x01;
    }

    return parity;
}

uint8_t Packet::payloadParity()
{
    uint8_t parity = 0;
    for (int i = 0; i < PAYLOAD_SIZE; i ++)
    {
        for (int j = 0; j < 8; j ++)
        {
            parity ^= (payload[i] >> j) & 0x01;
        }
    }

    return parity;
}
