#ifndef __SENSOR_PINS_H__
#define __SENSOR_PINS_H__

#define RADIO_CSN 9
#define RADIO_CE 10
#define RADIO_IRQ 14

#define GPS_PPS //Currently not used
#define GPS_FIX //Currently not used
#define GPS_EN  //Currently not used

#define MOTOR_NW 0
#define MOTOR_NE 1
#define MOTOR_SE 2
#define MOTOR_SW 3
#define NUM_MOTORS 4

#define MOTOR_NW_PIN 23
#define MOTOR_NE_PIN 22
#define MOTOR_SE_PIN 21
#define MOTOR_SW_PIN 20

#define GYRO_GINT // Gyro Interrupt Pin
#define GYRO_GRDY // Gyro Ready Pin
#define GYRO_LIN1 // Accelerometer Interrupt Pin 1
#define GYRO_LIN2 // Accelerometer Interrupt Pin 2
#define GYRO_LRDY // Accelerometer Ready Pin

#define BATT_MON1 A9

#endif
