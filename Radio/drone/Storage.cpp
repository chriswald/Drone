#include "Storage.h"

bool storage_write(uint32_t baseaddr, void* data, uint32_t length)
{
    if (baseaddr + length > MAX_EEPROM_ADDR)
        return false;

    for (uint32_t i = 0; i < length; i ++, baseaddr ++)
    {
        EEPROM.write(baseaddr, ((uint8_t*)data)[i]);
    }
    return true;
}

bool storage_read(uint32_t baseaddr, void* data, uint32_t length)
{
    if (baseaddr + length > MAX_EEPROM_ADDR)
        return false;

    for (uint32_t i = 0; i < length; i ++, baseaddr ++)
    {
        ((uint8_t*)data)[i] = EEPROM.read(baseaddr);
    }
    return true;
}

bool storage_erase(uint32_t baseaddr, uint32_t length)
{
    if (baseaddr + length > MAX_EEPROM_ADDR)
        return false;

    for (uint32_t i = 0; i < length; i ++, baseaddr ++)
    {
        EEPROM.write(baseaddr, 0);
    }
    return true;
}
