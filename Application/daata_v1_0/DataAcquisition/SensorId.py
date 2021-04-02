"""
This file stores all the information needed for creating sensor objects

To add a sensor, add an element to the SensorId dictionary with the keyword being the id of that sensor. This should
match the id that is found in the SensorId.h file in the comms utility (DAQ>Libraries>Sensor). Then add all the
required parameters for that sensor along with any additional optional parameters.

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
    }
}