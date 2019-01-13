#include "Radio.h"
#include "SPI.h"

uint8_t HardwareSPI::transfer(uint8_t data) {
    return SPI.transfer(data);
}

void HardwareSPI::begin() {
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_2XCLOCK_MASK);
}

void HardwareSPI::end() {
    // Do nothing
}

HardwareSPI hardwareSPI;
