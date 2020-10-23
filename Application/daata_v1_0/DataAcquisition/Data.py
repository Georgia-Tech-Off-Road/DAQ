import logging

from DataAcquisition.Sensors import *
from DataAcquisition.DerivedSensors import *


logger = logging.getLogger("DataAcquisition")


class Data:
    def __init__(self, lock):
        logger.debug("Data object is being initialized")
        self.is_connected = False
        self.lock = lock

        self.__data = dict()

        # Internal sensors
        self.__data['unix_time'] = Time(display_name='Unix Time', unit='Seconds', unit_short='s', is_external=False)
        self.__data['timestamp'] = Time(display_name='Timestamp', is_external=False)
        self.__data['time'] = Time(display_name="Time", unit='Seconds', unit_short='s', is_external=False)

        # External sensors
        self.__data['teensy_time'] = Time(display_name='Time Since Start', unit='Micro-seconds', unit_short='us')
        self.__data['engine_rpm'] = HESpeedSensor(0, display_name='Engine RPM')
        self.__data['secondary_rpm'] = HESpeedSensor(0, display_name='Secondary RPM')
        self.__data['fl_lds'] = LDS(0, display_name='Front Left LDS')
        self.__data['fr_lds'] = LDS(0, display_name='Front Right LDS')
        self.__data['bl_lds'] = LDS(0, display_name='Back Left LDS')
        self.__data['br_lds'] = LDS(0, display_name='Back Right LDS')

        # Derived sensors
        self.__data['wheel_speed'] = WheelSpeed(self.__data['secondary_rpm'])
        self.__data['car_speed'] = CarSpeed(self.__data['secondary_rpm'])

        logger.info("Data object successfully initialized")



    def get_most_recent_index(self):
        with self.lock:
            return self.__data['time'].most_recent_index

    def get_value(self, sensor_name, index):
        with self.lock:
            try:
                return self.__data[sensor_name].get_value(index)
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))
                return None

    def get_values(self, sensor_name, index, num_values):
        with self.lock:
            try:
                return self.__data[sensor_name].get_values(index, num_values)
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))
                return None

    def get_sensors(self, is_external=None, is_plottable=None, is_derived=None, is_connected=None):
        """
        Versatile function to get a list of sensors with specific attributes
        Ex. calling get_sensors(is_derived=False, is_connected=True) would return a list of
        all the sensors that are connected but not derived

        :param is_external: If the sensor relies on an external source (True/False, defaults to None)
        :param is_plottable: If the sensor can be plotted against time (True/False, defaults to None)
        :param is_derived: If the value of the 'sensor' is derived from other sensors (True/False, defaults to None)
        :param is_connected: If the sensor is connected (True/False, defaults to None)
        :return: A list of sensor key names
        """

        logger.debug("Getting a list of sensors")

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
            if is_connected is not None:
                if self.__data[sensor_name].is_connected != is_connected:
                    sensor_fits_params = False
            if sensor_fits_params:
                sensors.append(sensor_name)
        return sensors

    def get_display_name(self, sensor_name):
        logger.debug(logger.debug("Getting the display name for {}".format(sensor_name)))
        try:
            return self.__data[sensor_name].display_name
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_unit(self, sensor_name):
        logger.debug("Getting the unit for {}".format(sensor_name))
        try:
            return self.__data[sensor_name].unit
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_unit_short(self, sensor_name):
        logger.debug("Getting the unit_short for {}".format(sensor_name))
        try:
            return self.__data[sensor_name].unit_short
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_is_external(self, sensor_name):
        logger.debug("Getting if {} is external".format(sensor_name))
        try:
            return self.__data[sensor_name].is_external
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_is_plottable(self, sensor_name):
        logger.debug("Getting if {} is plottable".format(sensor_name))
        try:
            return self.__data[sensor_name].is_plottable
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_is_derived(self, sensor_name):
        logger.debug("Getting if {} is a derived sensor".format(sensor_name))
        try:
            return self.__data[sensor_name].is_derived
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def get_is_connected(self, sensor_name):
        logger.debug("Getting the connection status for {}".format(sensor_name))
        try:
            return self.__data[sensor_name].is_connected
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def set_connected(self, sensor_name):
        try:
            if not self.__data[sensor_name].is_connected:
                self.__data[sensor_name].is_connected = True
                logger.info("{} has been connected".format(sensor_name))
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def set_disconnected(self, sensor_name):
        try:
            if self.__data[sensor_name].is_connected:
                self.__data[sensor_name].is_connected = False
                logger.warning("{} has been disconnected".format(sensor_name))
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))

    def add_value(self, sensor_name, value):
        # Make sure to wrap this function in the lock as it is not thread-safe
        self.__data[sensor_name].add_value(value)

    def reset(self):
        logger.debug("Resetting all the sensors")
        sensors = self.get_sensors(is_derived=False)
        with self.lock:
            for sensor in sensors:
                self.__data[sensor].reset()
