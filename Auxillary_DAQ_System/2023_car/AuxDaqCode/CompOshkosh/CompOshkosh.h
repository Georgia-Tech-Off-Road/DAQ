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

// Control Libraries
#include "DigitalOutput.h"

// Utility
#include "ClockTimer.h"
#include "EdgeDetector.h"

// Pins and Constants
#define ADC1 33
#define ADC2 34
#define ADC3 35

#define LDS_PEDAL1        0
#define LDS_PEDAL2        1
#define LDS_PEDAL3        2
#define LDS_PEDAL4        3
#define LDS_PEDAL_STROKE 200

#define BRAKE_FRONT 4
#define BRAKE_REAR  5

#define SPEED_ENGINE        23
#define SPEED_ENGINE_PPR    600
#define SPEED_SECONDARY     22
#define SPEED_SECONDARY_PPR 30

#define IMU 26

#define LED_ONBOARD       30
#define LED_PANEL_WHITE   9
#define LED_PANEL_RED     8
#define BTN_PANEL         7

// Dashboard
#define DASHBOARD_SERIAL Serial4

#endif
