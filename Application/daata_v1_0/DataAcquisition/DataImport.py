import serial
import time
import random
import math

from DataAcquisition.Data import Data


class DataImport:
    def __init__(self, data, lock, is_data_collecting, use_fake_inputs):
        self.data = data
        self.lock = lock
        self.use_fake_inputs = use_fake_inputs
        self.is_data_collecting = is_data_collecting

        # Variables needed for fake data
        self.start_time = time.time()
        self.prev_engine_val = 1800
        self.prev_lds_val = 0

        self.temp_data = {}
        for sensor in data.get_sensors_from_param(is_external=True, is_derived=False):
            self.temp_data[sensor] = {'value': None, 'has_been_updated': False, 'is_used': False}

    def check_connected(self):
        if self.use_fake_inputs:
            self.check_connected_fake()
        # TODO implement actual serial reading

    def read_data(self):
        if self.use_fake_inputs:
            self.read_data_fake()
        # TODO implement actual serial reading

    def check_connected_fake(self):
        if time.time() - self.start_time > 2:
            self.data.is_connected = True

    def read_data_fake(self):
        with self.lock:
            if not self.data.is_connected:
                self.check_connected_fake()
            else:
                self.prev_engine_val = max(1800, min(4000, self.prev_engine_val + 500 * math.sin(
                    (time.time() - self.start_time) / 10) + random.randrange(-5, 5)))
                self.data.add_value('engine_rpm', self.prev_engine_val)
                self.prev_lds_val = 100 + 100 * math.sin((time.time() - self.start_time) / 20)
                self.data.add_value('fl_lds', self.prev_lds_val)



