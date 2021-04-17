from abc import ABCMeta, abstractmethod
import logging
import math


logger = logging.getLogger("DataAcquisition")


class Sensor(metaclass=ABCMeta):
    def __init__(self, **kwargs):
        self.values = list()
        self.name = kwargs.get('name')
        self.object = kwargs.get('object')
        self.most_recent_index = 0
        self.display_name = kwargs.get('display_name')
        self.unit = kwargs.get('unit')
        self.unit_short = kwargs.get('unit_short')
        self.is_plottable = kwargs.get('is_plottable', True)
        self.is_external = kwargs.get('is_external', True)
        self.is_derived = False
        self.is_connected = False
        self.is_float = kwargs.get('is_float')

    @abstractmethod
    def add_value(self, value):
        raise NotImplementedError

    def get_value(self, index):
        try:
            return self.values[index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def get_values(self, index, num_values):
        try:
            try:
                assert index - num_values >= 0
                return self.values[index - num_values:index]
            except AssertionError:
                logger.debug("Tried to get more values than are contained, returning all values")
                return self.values[0:index]
        except IndexError:
            logger.error("Index out of range, use get_most_recent_index to ensure that the index exists")
            return None

    def reset(self):
        try:
            logger.debug("Resetting the sensor {}".format(self.display_name))
            self.values = list()
            self.most_recent_index = 0
        except Exception as e:
            logger.error(e)


class Generic(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def add_value(self, value):
        try:
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class Flag(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.is_plottable = False

    def add_value(self, value):
        try:
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class Command(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.is_plottable = False

    def add_value(self, value):
        try:
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class Time(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.is_plottable = False

    def add_value(self, value):
        try:
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class SpeedPosition(Sensor):
    def __init__(self, pulses_per_revolution, **kwargs):
        super().__init__(**kwargs)
        self.ppr = pulses_per_revolution
        self.unit = kwargs.get('unit', "Revolutions Per Minute")
        self.unit_short = kwargs.get('unit_short', "RPM")

    def add_value(self, value):
        try:
            # TODO: Implement a transfer function for conversion to RPM
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class Pressure(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.unit = kwargs.get('unit', "Pounds Per Square Inch")
        self.unit_short = kwargs.get('unit_short', "PSI")

    def add_value(self, value):
        try:
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class Force(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.unit = kwargs.get('unit', "Pounds")
        self.unit_short = kwargs.get('unit_short', "lbs")

    def add_value(self, value):
        try:
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class LDS(Sensor):
    def __init__(self, stroke_length, **kwargs):
        super().__init__(**kwargs)
        self.stroke_length = stroke_length
        self.unit = kwargs.get('unit', "Millimeters")
        self.unit_short = kwargs.get('unit_short', "mm")

    def add_value(self, value):
        try:
            # TODO: Implement a transfer function for conversion to from raw data to mm
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)


class IMU(Sensor):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.unit = kwargs.get('unit', "Acceleration")
        self.unit_short = kwargs.get('unit_short', "m/s^2")

    def add_value(self, value):
        try:
            self.is_connected = True
            self.values.append(value)
            self.most_recent_index = len(self.values) - 1
        except Exception as e:
            logger.error(e)
