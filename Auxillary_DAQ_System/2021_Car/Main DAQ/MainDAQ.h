#ifndef MAIN_DAQ_H
#define MAIN_DAQ_H

#include <Arduino.h>
#include "HallEffectSpeedSensor.h"

#define 3V3DIG_1 14
#define 3V3DIG_2 5
#define 3V3DIG_3 16
#define 3V3DIG_4 6
#define 3V3DIG_5 15
#define 3V3DIG_1 2
#define 3V3DIG_1 38
#define 3V3DIG_1 3

#define RX_1 17
#define RX_2 39

#define 3V3ANA_1 9
#define 3V3ANA_2 22
#define 3V3ANA_3 33
#define 3V3ANA_4 23
#define 3V3ANA_5 7
#define 3V3ANA_6 4
#define 3V3ANA_7 40
#define 3V3ANA_8 32
#define 3V3ANA_9 41
#define 3V3ANA_10 8

#define SDA 18
#define SCL 19

#define MISO 12
#define MOSI 11
#define CS_1 13
#define CS_2 24
#define CS_3 25
#define CS_4 26
#define CS_5 27
#define CS_6 28
#define CS_7 29

#define 5VDIG_1 31
#define 5VDIG_2 30
#define 5VDIG_3 20
#define 5VDIG_4 21

#define 5VANA_1 AIN0
#define 5VANA_2 AIN2
#define 5VANA_3 AIN1
#define 5VANA_4 AIN3

// 3V3 Dig Pins (5, 2, 6, 3, 8, 4, 32, 33)

// TX (8) RX (7)

// 3V3 Analog Pins (14, 15, 16, 38, 17, 39, 22, 40, 23, 41)

// SDA (18) SDL (19)

// MISO (12), MOSI (11), CS (13, 24, 25, 26, 27, 28, 29)

// 5V Dig Pins (31, 20, 30, 21)

// 5V Analog Pins (AIN1-4) 

#endif