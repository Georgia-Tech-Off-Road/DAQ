import logging

from DataAcquisition.Sensors import *
from DataAcquisition.DerivedSensors import *
from DataAcquisition.SensorId import SensorId


logger = logging.getLogger("DataAcquisition")


class Data:
    def __init__(self, lock):
        with lock:
            logger.debug("Data object is being initialized")
            self.is_connected = False
            self.lock = lock

            # create dictionaries of Sensor objects
            self.__data = dict()
            for sensor_id in SensorId:
                try:
                    object_type = SensorId[sensor_id]["object"]
                    param_dict = SensorId[sensor_id]
                    param_dict["id"] = sensor_id
                    self.generate_object(SensorId[sensor_id]["name"], object_type, param_dict)
                except KeyError:
                    try:
                        i = 0
                        while True:
                            object_type = SensorId[sensor_id][i]["object"]
                            param_dict = SensorId[sensor_id][i]
                            param_dict["id"] = sensor_id
                            self.generate_object(SensorId[sensor_id][i]["name"], object_type, param_dict)
                            i = i + 1
                    except KeyError:
                        pass

            '''
            # Internal sensors
            self.__data['unix_time'] = Time(display_name='Unix Time', unit='Seconds', unit_short='s', is_external=False)
            self.__data['timestamp'] = Time(display_name='Timestamp', is_external=False)
            self.__data['time'] = Time(display_name="Time", unit='Seconds', unit_short='s', is_external=False)
            self.__data['flag_data_collecting'] = Command(id=1, display_name="Is Data Being Collected", is_external=False)
    
            # Derived sensors
            self.__data['wheel_speed'] = WheelSpeed(self.__data['secondary_rpm'])
            self.__data['car_speed'] = CarSpeed(self.__data['secondary_rpm'])
            '''

            logger.info("Data object successfully initialized")

    def generate_object(self, sensor_name, object_type, param_dict):
        """
        Used for generating the data object (only should be used internally)

        :param sensor_name: The key used for the data object
        :param object_type: The type of object to generate
        :param param_dict: The kwargs used for the object
        """
        logger.info("Generating {} of type: {}".format(sensor_name, object_type))
        if object_type == "Generic":
            self.__data[sensor_name] = Generic(**param_dict)
        if object_type == "Flag":
            self.__data[sensor_name] = Flag(**param_dict)
        if object_type == "Command":
            self.__data[sensor_name] = Command(**param_dict)
        if object_type == "Time":
            self.__data[sensor_name] = Time(**param_dict)
        if object_type == "Speed":
            self.__data[sensor_name] = Speed(**param_dict)
        if object_type == "Position":
            self.__data[sensor_name] = Position(**param_dict)
        if object_type == "LDS":
            self.__data[sensor_name] = LDS(**param_dict)
        if object_type == "Pressure":
            self.__data[sensor_name] = Pressure(**param_dict)
        if object_type == "Force":
            self.__data[sensor_name] = Force(**param_dict)
        if object_type == "Acceleration":
            self.__data[sensor_name] = Acceleration(**param_dict)
        if object_type == "Gyro":
            self.__data[sensor_name] = Gyro(**param_dict)
        if object_type == "Temperature":
            self.__data[sensor_name] = Temperature(**param_dict)
        if object_type == "WheelSpeed":
            self.__data[sensor_name] = WheelSpeed(**param_dict)
        if object_type == "CarSpeed":
            self.__data[sensor_name] = CarSpeed(**param_dict)

    def get_most_recent_index(self, sensor_name="time_internal_seconds"):
        with self.lock:
            try:
                return self.__data[sensor_name].most_recent_index
            except KeyError:
                logger.error("The sensor {} does not exist, check your spelling".format(sensor_name))
                return None

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
            if self.__data[sensor_name].display_name is None:
                return sensor_name
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

    def reset(self):
        logger.debug("Resetting all the sensors")
        sensors = self.get_sensors(is_derived=False)
        with self.lock:
            for sensor in sensors:
                self.__data[sensor].reset()

    # ---------------------------- Below are functions to only be used by DataImport ----------------------------
    def set_connected(self, sensor_id):
        try:
            self.__data[SensorId[sensor_id]["name"]].is_connected = True
            logger.info("{} has been connected".format(SensorId[sensor_id]["name"]))
        except KeyError:
            try:
                for key in SensorId[sensor_id]:
                    if isinstance(key, int):
                        self.__data[SensorId[sensor_id][key]["name"]].is_connected = True
                        logger.info("{} has been connected".format(SensorId[sensor_id][key]["name"]))
            except KeyError:
                logger.error("Key error occurred in add_value for sensor with ID: {}".format(sensor_id))
        except Exception as e:
            logger.error(e)

    def set_disconnected(self, sensor_id):
        try:
            self.__data[SensorId[sensor_id]["name"]].is_connected = False
            logger.info("{} has been disconnected".format(SensorId[sensor_id]["name"]))
        except KeyError:
            try:
                for key in SensorId[sensor_id]:
                    if isinstance(key, dict):
                        self.__data[SensorId[sensor_id][key]["name"]].is_connected = False
                        logger.info("{} has been disconnected".format(SensorId[sensor_id][key]["name"]))
            except KeyError:
                logger.error("Key error occurred in add_value for sensor with ID: {}".format(sensor_id))
        except Exception as e:
            logger.error(e)

    def add_value(self, sensor_id, value):
        # Make sure to wrap this function in the lock as it is not thread-safe
        try:
            self.__data[SensorId[sensor_id]["name"]].add_value(value)
        except KeyError:
            try:
                for i in range(len(value)):
                    self.__data[SensorId[sensor_id][i]["name"]].add_value(value[i])
            except KeyError:
                logger.error("Key error occurred in add_value for sensor with ID: {}".format(sensor_id))
        except Exception as e:
            logger.error(e)


