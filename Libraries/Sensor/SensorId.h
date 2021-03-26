#ifndef SENSORID_H
#define SENSORID_H

#include <Arduino.h>

enum sensor_id_t : uint16_t
{
    /**
     * 000 - DEFAULTS, FLAGS, COMMANDS, MISC
     */
    DEFAULT_NO_SENSOR          =  0,
    FLAG_DATACOLLECTING        =  1,
    COMMAND_SDCARDFILENAME_STR =  2,
    TEST_SENSOR_0              = 90,
    TEST_SENSOR_1              = 91,
    TEST_SENSOR_2              = 92,
    TEST_SENSOR_3              = 93,
    TEST_SENSOR_4              = 94,
    TEST_SENSOR_5              = 95,
    TEST_SENSOR_6              = 96,
    TEST_SENSOR_7              = 97,
    TEST_SENSOR_8              = 98,
    TEST_SENSOR_9              = 99,

    /**
     * 100 - TIME "SENSORS"
     */
    TIME_GENERIC   = 100,
    TIME_DASH_MS   = 101,
    TIME_DASH_US   = 102,
    TIME_AUXDAQ_MS = 103,
    TIME_AUXDAQ_US = 104,
    TIME_DIFF_MS   = 105,
    TIME_DIFF_US   = 106,
    TIME_DAATA_MS  = 107,
    TIME_DAATA_US  = 108,

    /**
     * 200 - SPEED SENSORS
     */
    SPEED_GENERIC       = 200,
    SPEED_ENGINE_RPM    = 201,
    SPEED_SECONDARY_RPM = 202,

    /**
     * 400 - LDS SENSORS
     */
    LDS_GENERIC            = 300,
    LDS_FRONTLEFTSHOCK_MM  = 301,
    LDS_FRONTRIGHTSHOCK_MM = 302,
    LDS_BACKLEFTSHOCK_MM   = 303,
    LDS_BACKRIGHTSHOCK_MM  = 304
};

#endif