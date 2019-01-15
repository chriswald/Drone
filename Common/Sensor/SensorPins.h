#ifndef __SENSOR_PINS_H__
#define __SENSOR_PINS_H__

#if defined PLATFORM_TEENSY
#define RADIO_CE    10
#define RADIO_CSN   9
#define RADIO_IRQ   14
#elif defined PLATFORM_ARDUINO
#define RADIO_CE    8
#define RADIO_CSN   7
#define RADIO_IRQ   2
#endif


#endif // __SENSOR_PINS_H__