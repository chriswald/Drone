#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <stdint.h>
#include <EEPROM.h>

#define MAX_EEPROM_ADDR 2048

bool storage_write(uint32_t baseaddr, void* data, uint32_t length);
bool storage_read (uint32_t baseaddr, void* data, uint32_t length);
bool storage_erase(uint32_t baseaddr=0, uint32_t length=MAX_EEPROM_ADDR);

#endif //__STORAGE_H__
