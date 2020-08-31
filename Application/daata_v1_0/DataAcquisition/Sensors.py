from abc import ABCMeta, abstractmethod
import logging
import math


logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class Sensor(metaclass=ABCMeta):
    @abstractmethod
    def get_value(self, index):
        raise NotImplementedError

    @abstractmethod
    def get_values(self, index, num_values):
        raise NotImplementedError

    @abstractmethod
    def add_value(self, value):
        raise NotImplementedError

    @abstractmethod
    def reset(self):
        raise NotImplementedError


class Time(Sensor):
    def __init__(self, display_name, unit, unit_short, is_external):
        self.__values = list()
        self.most_recent_index = 0
        self.display_name = display_name
        self.unit = unit
        self.unit_short = unit_short
        self.is_plottable = False
        self.is_external = is_external
        self.is_derived = False
        self.is_connected = False

    def get_value(self, index):
        try:
            return self.__values[index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def get_values(self, index, num_values):
        try:
            try:
                assert index-num_values >= 0
                return self.__values[index-num_values:index]
            except AssertionError:
                logger.debug("Tried to get more values than are contained, returning all values")
                return self.__values[0:index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def add_value(self, value):
        try:
            self.__values.append(value)
            self.most_recent_index = len(self.__values) - 1
        except Exception as e:
            logger.error(e)

    def reset(self):
        try:
            self.__values = []
            self.most_recent_index = 0
        except Exception as e:
            logger.error(e)


class HESpeedSensor(Sensor):
    def __init__(self, display_name, pulses_per_revolution):
        self.__values = list()
        self.most_recent_index = 0
        self.display_name = display_name
        self.unit = "Revolutions Per Minute"
        self.unit_short = "RPM"
        self.is_plottable = True
        self.is_external = True
        self.is_derived = False
        self.is_connected = False

    def get_value(self, index):
        try:
            return self.__values[index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def get_values(self, index, num_values):
        try:
            try:
                assert index-num_values >= 0
                return self.__values[index-num_values:index]
            except AssertionError:
                logger.debug("Tried to get more values than are contained, returning all values")
                return self.__values[0:index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def add_value(self, value):
        try:
            # TODO: Implement a transfer function for conversion to RPM
            self.__values.append(value)
            self.most_recent_index = len(self.__values) - 1
        except Exception as e:
            logger.error(e)

    def reset(self):
        try:
            self.__values = []
            self.most_recent_index = 0
        except Exception as e:
            logger.error(e)


class LDS(Sensor):
    def __init__(self, display_name, stroke_length):
        self.__values = list()
        self.most_recent_index = 0
        self.display_name = display_name
        self.unit = "Millimeters"
        self.unit_short = "mm"
        self.is_plottable = True
        self.is_external = True
        self.is_derived = False
        self.is_connected = False

    def get_value(self, index):
        try:
            return self.__values[index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def get_values(self, index, num_values):
        try:
            try:
                assert index-num_values >= 0
                return self.__values[index-num_values:index]
            except AssertionError:
                logger.debug("Tried to get more values than are contained, returning all values")
                return self.__values[0:index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def add_value(self, value):
        try:
            # TODO: Implement a transfer function for conversion to from raw data to mm
            self.__values.append(value)
            self.most_recent_index = len(self.__values) - 1
        except Exception as e:
            logger.error(e)

    def reset(self):
        try:
            self.__values = []
            self.most_recent_index = 0
        except Exception as e:
            logger.error(e)
