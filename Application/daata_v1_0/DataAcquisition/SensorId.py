"""
This file stores all the information needed for creating sensor objects

To add a sensor, add an element to the SensorId dictionary with the 
keyword being the id of that sensor. This shouldmatch the id that is 
found in the SensorId.h file in the comms utility (DAQ>Libraries>Sensor). 
Then add all the required parameters for that sensor along with any 
additional optional parameters.

Required parameters:
    - name
    - object

Optional parameters:
    - display_name (defaults to None)
    - unit         (defaults to None)
    - unit_short   (defaults to None)
    - is_plottable (defaults to True)
    - is_external  (defaults to True)
    - class        (defaults to None)


Current classes:
    - dashboard
    - aux_daq
    - differential
    - engine_dyno
    - shock_dyno


"""


SensorId = {
    # 000 - DEFAULTS, FLAGS, COMMANDS, MISC
    0: {
        "name": "default_no_sensor",
        "object": "Generic"
    },
    1: {
        "name": "flag_datacollecting",
        "object": "Flag"
    },
    2: {
        "name": "command_sdcardfilenamestring",
        "object": "Command"
    },



    # 100 - TIME "SENSORS"
    100: {
        "name": "time_generic",
        "object": "Time"
    },
    101: {
        "name": "time_dash_ms",
        "object": "Time",
        "display_name": "Dashboard Time Since Start",
        "unit": "Miliseconds",
        "unit_short": "ms",
        "class": "dashboard"
    },
    102: {
        "name": "time_dash_us",
        "object": "Time"
    },
    103: {
        "name": "time_auxdaq_ms",
        "object": "Time"
    },
    104: {
        "name": "time_auxdaq_us",
        "object": "Time"
    },
    105: {
        "name": "time_diff_ms",
        "object": "Time"
    },
    106: {
        "name": "time_diff_us",
        "object": "Time"
    },
    107: {
        "name": "time_daata_ms",
        "object": "Time"
    },
    108: {
        "name": "time_daata_us",
        "object": "Time"
    },
    199: {
        "name": "rtc_unixtime",
        "object": "Time"
    },



    # 200 - SPEED SENSORS
    200: {
        "name": "speed_generic",
        "object": "SpeedSensor",
        "pulses_per_revolution": 1
    },
    201: {
        "name": "speed_engine_rpm",
        "object": "SpeedSensor",
        "pulses_per_revolution": 600
    },
    202: {
        "name": "speed_secondary_rpm",
        "object": "SpeedSensor",
        "pulses_per_revolution": 30
    },
    
    # BRAKE SENSORS
    203: {
        "name": "brake_sensor1",
        "object": "BrakeSensor"
    },
    204: {
        "name": "brake_sensor2",
        "object": "BrakeSensor"
    },



    # 400 - LDS SENSORS
    205: {
        "name": "lds_generic",
        "object": "LDS",
        "stroke_length": 1
    },
    206: {
        "name": "lds_frontleftshock_mm",
        "object": "LDS",
        "stroke_length": 200
    },
    207: {
        "name": "lds_frontrightshock_mm",
        "object": "LDS",
        "stroke_length": 200
    },
    208: {
        "name": "lds_backleftshock_mm",
        "object": "LDS",
        "stroke_length": 225
    },
    209: {
        "name": "lds_backrightshock_mm",
        "object": "LDS",
        "stroke_length": 225
    },

    220: {
        "name": "imu_sensor",
        "object": "IMU"
    }
}