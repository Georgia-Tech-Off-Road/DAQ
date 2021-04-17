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
    - num_bytes

Optional parameters:
    - h_file_comment (not stored within Sensor)
    - is_float       (defaults to None)
    - display_name   (defaults to None)
    - unit           (defaults to None)
    - unit_short     (defaults to None)
    - is_plottable   (defaults to True)
    - is_external    (defaults to True)
    - class          (defaults to None)


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
        "object": "Generic",
        "num_bytes": 0
    },
    1: {
        "name": "flag_datacollecting",
        "object": "Flag",
        "num_bytes": 1
    },
    2: {
        "name": "command_sdcardfilenamestring",
        "object": "Command",
        "num_bytes": 12
    },
    90: {
        "name": "test_sensor_0",
        "object": "Generic",
        "num_bytes": 4
    },
    91: {
        "name": "test_sensor_1",
        "object": "Generic",
        "num_bytes": 4
    },
    92: {
        "name": "test_sensor_2",
        "object": "Generic",
        "num_bytes": 4
    },
    93: {
        "name": "test_sensor_3",
        "object": "Generic",
        "num_bytes": 4
    },
    94: {
        "name": "test_sensor_4",
        "object": "Generic",
        "num_bytes": 4
    },
    95: {
        "name": "test_sensor_5",
        "object": "Generic",
        "num_bytes": 4
    },
    96: {
        "name": "test_sensor_6",
        "object": "Generic",
        "num_bytes": 4
    },
    97: {
        "name": "test_sensor_7",
        "object": "Generic",
        "num_bytes": 4
    },
    98: {
        "name": "test_sensor_8",
        "object": "Generic",
        "num_bytes": 4
    },
    99: {
        "name": "test_sensor_9",
        "object": "Generic",
        "num_bytes": 4
    },



    # 100 - TIME "SENSORS"
    100: {
        "name": "time_generic",
        "object": "Time",
        "num_bytes": 4
    },
    101: {
        "name": "time_dash_ms",
        "object": "Time",
        "num_bytes": 2,
        "display_name": "Dashboard Time Since Start",
        "unit": "Miliseconds",
        "unit_short": "ms",
        "class": "dashboard"
    },
    102: {
        "name": "time_dash_us",
        "object": "Time",
        "num_bytes": 4,
        "class": "dashboard"
    },
    103: {
        "name": "time_auxdaq_ms",
        "object": "Time",
        "num_bytes": 2,
        "class": "aux_daq"
    },
    104: {
        "name": "time_auxdaq_us",
        "object": "Time",
        "num_bytes": 4,
        "class": "aux_daq"
    },
    105: {
        "name": "time_diff_ms",
        "object": "Time",
        "num_bytes": 2
    },
    106: {
        "name": "time_diff_us",
        "object": "Time",
        "num_bytes": 4
    },
    107: {
        "name": "time_daata_ms",
        "object": "Time",
        "num_bytes": 2
    },
    108: {
        "name": "time_daata_us",
        "object": "Time",
        "num_bytes": 4
    },
    199: {
        "name": "rtc_unixtime",
        "object": "Time",
        "num_bytes": 4
    },



    # 200 - SPEED/POSITION SENSORS
    200: {
        "name": "speed_generic",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "pulses_per_revolution": 1
    },
    201: {
        "name": "position_generic",
        "object": "SpeedPosition",
        "num_bytes": 4,
        "pulses_per_revolution": 1
    },
    202: {
        "name": "speed_position_generic4",
        "object": "SpeedPosition",
        "num_bytes": [2, 4],
        "h_file_comment": "Speed in RPM and position in ticks (4ppr sensor)",
        "display_name": ["Speed", "Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 4
    },
    203: {
        "name": "speed_position_generic30",
        "object": "SpeedPosition",
        "num_bytes": [2, 4],
        "h_file_comment": "Speed in RPM and position in ticks (30ppr sensor)",
        "display_name": ["Speed", "Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 30
    },
    204: {
        "name": "speed_position_generic500",
        "object": "SpeedPosition",
        "num_bytes": [2, 4],
        "h_file_comment": "Speed in RPM and position in ticks (500ppr sensor)",
        "display_name": ["Speed", "Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 500
    },
    205: {
        "name": "speed_position_generic600",
        "object": "SpeedPosition",
        "num_bytes": [2, 4],
        "h_file_comment": "Speed in RPM and position in ticks (600ppr sensor)",
        "display_name": ["Speed", "Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 600
    },
    206: {
        "name": "speed_position_engine600",
        "object": "SpeedPosition",
        "num_bytes": [4, 4],
        "h_file_comment": "Speed in RPM and position in ticks (600ppr sensor)",
        "display_name": ["Engine Speed", "Engine Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 600,
        "class": "dashboard"
    },
    207: {
        "name": "speed_position_engine4",
        "object": "SpeedPosition",
        "num_bytes": [4, 4],
        "h_file_comment": "Speed in RPM and position in ticks (4ppr sensor)",
        "display_name": ["Engine Speed", "Engine Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 4,
        "class": "dashboard"
    },
    208: {
        "name": "speed_position_secondary30",
        "object": "SpeedPosition",
        "num_bytes": [4, 4],
        "h_file_comment": "Speed in RPM and position in ticks (30ppr on gear)",
        "display_name": ["Secondary Speed", "Secondary Position"],
        "unit": ["Revolutions Per Minutes", "Ticks"],
        "unit_short": ["RPM", "Ticks"],
        "pulses_per_revolution": 30,
        "class": "dashboard"
    },
    209: {
        "name": "speed_engine600_rpm",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "h_file_comment": "Speed in RPM (600ppr sensor)",
        "display_name": "Engine Speed",
        "unit": "Revolutions Per Minutes",
        "unit_short": "RPM",
        "pulses_per_revolution": 600,
        "class": "dashboard"
    },
    210: {
        "name": "speed_engine4_rpm",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "h_file_comment": "Speed in RPM (4ppr sensor)",
        "display_name": "Engine Speed",
        "unit": "Revolutions Per Minutes",
        "unit_short": "RPM",
        "pulses_per_revolution": 4,
        "class": "dashboard"
    },
    211: {
        "name": "speed_secondary30_rpm",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "h_file_comment": "Speed in RPM (30ppr sensor)",
        "display_name": "Engine Speed",
        "unit": "Revolutions Per Minutes",
        "unit_short": "RPM",
        "pulses_per_revolution": 30,
        "class": "dashboard"
    },
    212: {
        "name": "speed_dynoengine600_rpm",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "h_file_comment": "Speed in RPM (600ppr sensor)",
        "display_name": "Engine Speed",
        "unit": "Revolutions Per Minutes",
        "unit_short": "RPM",
        "pulses_per_revolution": 600,
        "class": "engine_dyno"
    },
    213: {
        "name": "speed_dynosecondary30_rpm",
        "object": "SpeedPosition",
        "num_bytes": 2,
        "h_file_comment": "Speed in RPM (30ppr sensor)",
        "display_name": "Engine Speed",
        "unit": "Revolutions Per Minutes",
        "unit_short": "RPM",
        "pulses_per_revolution": 30,
        "class": "engine_dyno"
    },
    
    # 300 - FORCE/PRESSURE SENSORS
    300: {
        "name": "pressure_generic",
        "object": "Pressure",
        "num_bytes": 2
    },
    301: {
        "name": "force_generic",
        "object": "Force",
        "num_bytes": 4,
        "is_float": True
    },
    302: {
        "name": "brake_sensor1",
        "object": "Pressure",
        "num_bytes": 4,
        "class": "aux_daq"
    },
    303: {
        "name": "brake_sensor2",
        "object": "Pressure",
        "num_bytes": 4,
        "class": "aux_daq"
    },
    304: {
        "name": "pressure_frontbrake_psi",
        "object": "Pressure",
        "num_bytes": 2,
        "class": "aux_daq"
    },
    305: {
        "name": "pressure_rearbrake_psi",
        "object": "Pressure",
        "num_bytes": 2,
        "class": "aux_daq"
    },
    306: {
        "name": "force_dyno_lbs",
        "object": "Force",
        "num_bytes": 4,
        "is_float": True,
        "class": "engine_dyno"
    },



    # 400 - LDS SENSORS
    400: {
        "name": "lds_generic",
        "object": "LDS",
        "num_bytes": 2,
        "stroke_length": 1
    },
    401: {
        "name": "lds_frontleftshock_mm",
        "object": "LDS",
        "num_bytes": 2,
        "stroke_length": 200,
        "class": "aux_daq"
    },
    402: {
        "name": "lds_frontrightshock_mm",
        "object": "LDS",
        "num_bytes": 2,
        "stroke_length": 200,
        "class": "aux_daq"
    },
    403: {
        "name": "lds_backleftshock_mm",
        "object": "LDS",
        "num_bytes": 2,
        "stroke_length": 225,
        "class": "aux_daq"
    },
    404: {
        "name": "lds_backrightshock_mm",
        "object": "LDS",
        "num_bytes": 2,
        "stroke_length": 225,
        "class": "aux_daq"
    },

    # 500 - IMU SENSORS
    500: {
        "name": "imu_sensor",
        "object": "IMU",
        "num_bytes": [4, 4, 4, 4, 4, 4, 4],
        "is_float": [True, True, True, True, True, True, True],
        "h_file_comment": "Accel X, Y, Z; Gyro X, Y, Z; Temp",
        "class": "aux_daq"
    }
}