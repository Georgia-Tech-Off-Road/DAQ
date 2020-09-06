from abc import ABCMeta, abstractmethod, abstractproperty
import logging
import math


logger = logging.getLogger("DataAcquisition")


class DerivedSensor(metaclass=ABCMeta):
    def __init__(self, **kwargs):
        self.display_name = kwargs.get('display_name')
        self.unit = kwargs.get('unit')
        self.unit_short = kwargs.get('unit_short')
        self.is_plottable = kwargs.get('is_plottable', True)
        self.is_external = kwargs.get('is_external', True)  # If it relies on external sensors consider it external
        self.is_derived = True

    @abstractproperty
    def is_connected(self):
        raise NotImplementedError


class WheelSpeed(DerivedSensor):
    def __init__(self, secondary_speed, **kwargs):
        super().__init__(**kwargs)
        self.__values = list()
        self.most_recent_index = 0
        self.display_name = "Wheel Speed"
        self.unit = 'Revolutions Per Minute'
        self.unit_short = 'RPM'
        self.secondary_speed = secondary_speed
        self.gearbox_ratio = 0  # TODO: Need to get the gearbox ratio

    def get_value(self, index):
        return self.secondary_speed.get_value(index) * self.gearbox_ratio

    def get_values(self, index, num_values):
        return [value * self.gearbox_ratio for value in self.secondary_speed.get_values(index, num_values)]

    @property
    def is_connected(self):
        return self.secondary_speed.is_connected


class CarSpeed(DerivedSensor):
    def __init__(self, secondary_speed, **kwargs):
        super().__init__(**kwargs)
        self.__values = list()
        self.most_recent_index = 0
        self.display_name = "Car Speed"
        self.unit = 'Miles Per Hour'
        self.unit_short = 'MPH'
        self.secondary_speed = secondary_speed
        self.transfer_function = 0  # TODO: Need to get the transfer function

    def get_value(self, index):
        return self.secondary_speed.get_value(index) * self.transfer_function

    def get_values(self, index, num_values):
        return [value * self.transfer_function for value in self.secondary_speed.get_values(index, num_values)]

    @property
    def is_connected(self):
        return self.secondary_speed.is_connected


