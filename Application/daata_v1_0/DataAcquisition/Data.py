import logging

from DataAcquisition.Sensors import *
from DataAcquisition.DerivedSensors import *
from DataAcquisition.SensorId import SensorId


logger = logging.getLogger("DataAcquisition")


class Data:
    def __init__(self, lock):
        logger.debug("Data object is being initialized")
        self.is_connected = False
        self.lock = lock

        # create dictionaries of Sensor objects
        self.__data = dict()
        for sensor_id in SensorId:
            try:
                object_type = SensorId[sensor_id]["object"]
            except KeyError:
                logger.error("Sensor with ID {} missing required parameters".format(sensor_id))
                break
            if object_type == "Generic":
                self.__data[sensor_id] = Generic(**SensorId[sensor_id])
            if object_type == "Flag":
                self.__data[sensor_id] = Flag(**SensorId[sensor_id])
            if object_type == "Command":
                self.__data[sensor_id] = Command(**SensorId[sensor_id])
            if object_type == "Time":
                self.__data[sensor_id] = Time(**SensorId[sensor_id])
            if object_type == "SpeedPosition":
                self.__data[sensor_id] = SpeedPosition(**SensorId[sensor_id])
            if object_type == "LDS":
                self.__data[sensor_id] = LDS(**SensorId[sensor_id])
            if object_type == "Pressure":
                self.__data[sensor_id] = Pressure(**SensorId[sensor_id])
            if object_type == "Force":
                self.__data[sensor_id] = Force(**SensorId[sensor_id])
            if object_type == "IMU":
                self.__data[sensor_id] = IMU(**SensorId[sensor_id])
            if object_type == "WheelSpeed":
                self.__data[sensor_id] = WheelSpeed(**SensorId[sensor_id])
            if object_type == "CarSpeed":
                self.__data[sensor_id] = CarSpeed(**SensorId[sensor_id])

        # TODO: remove this stuff once SensorId list has been filled out
        '''
        # Internal sensors
        self.__data['unix_time'] = Time(display_name='Unix Time', unit='Seconds', unit_short='s', is_external=False)
        self.__data['timestamp'] = Time(display_name='Timestamp', is_external=False)
        self.__data['time'] = Time(display_name="Time", unit='Seconds', unit_short='s', is_external=False)
        self.__data['flag_data_collecting'] = Command(sensor_id=1, display_name="Is Data Being Collected", is_external=False)

        # External sensors
        self.__data['teensy_time'] = Time(sensor_id=100, display_name='Time Since Start', unit='Micro-seconds', unit_short='us')
        self.__data['time_dash_ms'] = Time(sensor_id=101, display_name='Dashboard Time Since Start', unit='Miliseconds', unit_short='ms')

        # Derived sensors
        self.__data['wheel_speed'] = WheelSpeed(self.__data['secondary_rpm'])
        self.__data['car_speed'] = CarSpeed(self.__data['secondary_rpm'])
        '''

        logger.info("Data object successfully initialized")

    def get_most_recent_index(self):
        """
        Gives the index of the last value that was collected

        :return:  most_recent_index The index of the most recent value that was collected
        """

        with self.lock:
            return self.__data['time'].most_recent_index

    def get_value(self, sensor_id, index):
        """
        Safely gives the value for a sensor id at a given index (the most recent index can be retrieved with
        get_most_recent_index).

        :param sensor_id: The ID of the sensor to get data from
        :param index: The index to retrieve the data from
        :return: value The measured value of the sensor at that index
        """

        with self.lock:
            try:
                return self.__data[sensor_id].get_value(index)
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))
                return None

    def get_values(self, sensor_id, index, num_values):
        """
        Safely gives a list of values for a sensor id at a given index (the most recent index can be retrieved with
        get_most_recent_index). If get_values(1, 10, 5) is called, it will give 5 values of the sensor with ID 1
        starting at position 10 (i.e. it will give the data points at positions 10, 9, 8, 7, 6).

        :param sensor_id: The ID of the sensor to get data from
        :param index: The index to retrieve the data from
        :param num_values: The number of values to measure
        :return: values: The list of values measured from the sensor at a given index
        """

        with self.lock:
            try:
                return self.__data[sensor_id].get_values(index, num_values)
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))
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
        for sensor_id in self.__data.keys():
            sensor_fits_params = True
            if is_external is not None:
                if self.__data[sensor_id].is_external != is_external:
                    sensor_fits_params = False
            if is_plottable is not None:
                if self.__data[sensor_id].is_plottable != is_plottable:
                    sensor_fits_params = False
            if is_derived is not None:
                if self.__data[sensor_id].is_derived != is_derived:
                    sensor_fits_params = False
            if is_connected is not None:
                if self.__data[sensor_id].is_connected != is_connected:
                    sensor_fits_params = False
            if sensor_fits_params:
                sensors.append(sensor_id)
        return sensors

    def get_display_name(self, sensor_id):
        """
        Gives the display name of a certain sensor (to be used for a graph title or whatever else).

        :param sensor_id: The ID of the sensor to get the display name of
        :return: The display name (string)
        """

        logger.debug(logger.debug("Getting the display name for {}".format(sensor_id)))
        try:
            return self.__data[sensor_id].display_name
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_unit(self, sensor_id):
        logger.debug("Getting the unit for {}".format(sensor_id))
        try:
            return self.__data[sensor_id].unit
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_unit_short(self, sensor_id):
        logger.debug("Getting the unit_short for {}".format(sensor_id))
        try:
            return self.__data[sensor_id].unit_short
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_is_external(self, sensor_id):
        logger.debug("Getting if {} is external".format(sensor_id))
        try:
            return self.__data[sensor_id].is_external
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_is_plottable(self, sensor_id):
        logger.debug("Getting if {} is plottable".format(sensor_id))
        try:
            return self.__data[sensor_id].is_plottable
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_is_derived(self, sensor_id):
        logger.debug("Getting if {} is a derived sensor".format(sensor_id))
        try:
            return self.__data[sensor_id].is_derived
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_is_connected(self, sensor_id):
        logger.debug("Getting the connection status for {}".format(sensor_id))
        try:
            return self.__data[sensor_id].is_connected
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def get_id_from_sensor_name(self, sensor_name):
        for sensor_id in self.__data.keys():
            if self.__data[sensor_id].name is sensor_name:
                return sensor_id
        logger.error("get_id_from_sensor_name failed because there was no sensor with name {}".format(sensor_name))

    def set_connected(self, sensor_id):
        try:
            if not self.__data[sensor_id].is_connected:
                self.__data[sensor_id].is_connected = True
                logger.info("{} has been connected".format(sensor_id))
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def set_disconnected(self, sensor_id):
        try:
            if self.__data[sensor_id].is_connected:
                self.__data[sensor_id].is_connected = False
                logger.warning("{} has been disconnected".format(sensor_id))
        except KeyError:
            logger.error("The sensor {} does not exist, check your spelling".format(sensor_id))

    def add_value(self, value, sensor_id=None, sensor_name=None):
        """
        Adds a value to a given sensor through it's name or it's id.

        Make sure to wrap this function in the lock as it is not thread-safe.
        This function can not be locked from within because generally lots of values are added at once and it is
        desirable to hold the lock while an entire packet is being written which has to be done from outside of this
        function.

        :param value: The value to be appended to the sensor's data
        :param sensor_name: The name of the sensor to add the value to (only needed if no sensor_id is given)
        :param sensor_id: The sensor_id of the sensor to add the value to (only needed if no sensor_name is given)
        """

        if sensor_id is not None and sensor_name is not None:
            logger.warning("An ID and sensor_name were both given for sensor with ID {}. Choosing the ID".format(sensor_id))
        if sensor_id is not None:
            try:
                self.__data[sensor_id].add_value(value)
            except KeyError:
                logger.error("No sensor found with ID {}".format(sensor_id))
        elif sensor_name is not None:
            for sensor_id in self.__data.keys():
                if self.__data[sensor_id].name is sensor_name:
                    self.__data[sensor_id].add_value(value)
        else:
            logger.error("add_value failed because no sensor_name or sensor_id was given")

    def reset(self):
        logger.debug("Resetting all the sensors")
        sensors = self.get_sensors(is_derived=False)
        with self.lock:
            for sensor in sensors:
                self.__data[sensor].reset()
