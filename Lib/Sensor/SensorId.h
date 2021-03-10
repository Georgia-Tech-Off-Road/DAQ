#ifndef SENSORID_H
#define SENSORID_H

#include <Arduino.h>

enum sensor_id_t : uint16_t
{
    /**
     * 000 - DEFAULTS, FLAGS, COMMANDS, MISC
     */
    DEFAULT_NO_SENSOR          = 000,
    FLAG_DATACOLLECTING        = 001,
    COMMAND_SDCARDFILENAME_STR = 002,

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