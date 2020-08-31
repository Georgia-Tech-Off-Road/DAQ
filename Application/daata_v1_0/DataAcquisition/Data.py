import logging

from DataAcquisition.Sensors import *
from DataAcquisition.DerivedSensors import *


logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class Data:
    def __init__(self, lock):
        self.is_connected = False
        self.lock = lock

        self.__data = dict()

        # Internal sensors
        self.__data['unix_time'] = Time('Unix Time', 'Seconds', 's', is_external=False)
        self.__data['timestamp'] = Time('Timestamp', None, None, is_external=False)

        # External sensors
        self.__data['teensy_time'] = Time('Time Since Start', 'Micro-seconds', 'us', is_external=True)
        self.__data['engine_rpm'] = HESpeedSensor('Engine RPM', 0)
        self.__data['secondary_rpm'] = HESpeedSensor('Secondary RPM', 0)
        self.__data['fl_lds'] = LDS('Front Left LDS', 0)
        self.__data['fr_lds'] = LDS('Front Right LDS', 0)
        self.__data['bl_lds'] = LDS('Back Left LDS', 0)
        self.__data['br_lds'] = LDS('Back Right LDS', 0)

        # Derived sensors
        self.__data['wheel_speed'] = WheelSpeed(self.__data['secondary_rpm'])
        self.__data['car_speed'] = CarSpeed(self.__data['secondary_rpm'])

    def get_most_recent_index(self):
        with self.lock:
            return self.__data['unix_time'].most_recent_index

    def get_value(self, sensor_name, index):
        with self.lock:
            try:
                self.__data[sensor_name].get_value(index)
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_values(self, sensor_name, index, num_values):
        # TODO implement get_values
        pass

    def get_sensors_from_param(self, is_external=None, is_plottable=None, is_derived=None):
        sensors = list()
        for sensor_name in self.__data.keys():
            sensor_fits_params = True
            if is_external is not None:
                if self.__data[sensor_name].is_external != is_external:
                    sensor_fits_params = False
            if is_plottable is not None:
                if self.__data[sensor_name].is_plottable != is_plottable:
                    sensor_fits_params = False
            if is_derived is not None:
                if self.__data[sensor_name].is_derived != is_derived:
                    sensor_fits_params = False
            if sensor_fits_params:
                sensors.append(sensor_name)
        return sensors

    def add_value(self, sensor_name, value):
        # Make sure to wrap this function in the lock as it is not thread-safe
        self.__data[sensor_name].add_value(value)

    def reset(self):
        # TODO implement reset
        pass
