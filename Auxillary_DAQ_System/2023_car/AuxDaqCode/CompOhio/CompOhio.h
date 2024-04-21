#ifndef COMPOSHKOSH_H
#define COMPOSHKOSH_H

// Communication Libraries
#include "SoftwareSerial.h"
#include "SerialComms.h"
#include "WirelessComms.h"
#include "SDComms.h"
#include "BlockId.h"

// Sensor Libraries
#include "TeensyADC.h"
#include "ADS8688.h"
#include "SpeedSensor.h"
#include "LDS.h"
#include "IMUSensor.h"
#include "TimeSensor.h"
#include "VoltageSensor.h"
#include "DigitalSensor.h"
#include "BrakePressureSensor.h"
#include "GPSSensor.h"
#include "WT901.h"

// Control Libraries
#include "DigitalOutput.h"

// Utility
#include "ClockTimer.h"
#include "EdgeDetector.h"
#include "../../../../LED_strip/arduinoLED/arduinoLED.h"
#include "../../../../LED_strip/OctoWS2811/OctoWS2811.h"



// Pins and Constants
#define LED_STRIP1 14
#define BYTES_PER_LED

#define ADC1 33
#define ADC2 34
#define ADC3 35

#define LDS_PEDAL1        0
#define LDS_PEDAL2        1
#define LDS_PEDAL3        2
#define LDS_PEDAL4        3
#define LDS_PEDAL_STROKE 200

#define BRAKE_FRONT 0
#define BRAKE_REAR  2

#define SPEED_ENGINE        17
#define SPEED_ENGINE_PPR    30
#define SPEED_SECONDARY     16
#define SPEED_SECONDARY_PPR 600
#define IMU 26

#define SPEED_WHEEL_L 21
#define SPEED_WHEEL_R 20
#define SPEED_WHEEL_PPR 30


#define LED_ONBOARD       30
#define LED_PANEL_WHITE   9
#define LED_PANEL_RED     8
#define BTN_PANEL         7

// Dashboard
#define DASHBOARD_SERIAL Serial4

#endif
