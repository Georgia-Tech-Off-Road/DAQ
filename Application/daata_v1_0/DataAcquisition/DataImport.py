import sys
import serial
from serial.tools import list_ports
import time
import logging
from datetime import datetime
import random
import math

from DataAcquisition.Data import Data

logger = logging.getLogger("DataAcquisition")

class DataImport:
    def __init__(self, data, lock, is_data_collecting, use_fake_inputs):
        self.data = data
        self.lock = lock
        self.use_fake_inputs = use_fake_inputs
        self.is_data_collecting = is_data_collecting
        self.start_time = datetime.now()
        
        self.teensy_port = self.get_Teensy_port()
        self.teensy_ser = serial.Serial(port=Teensy_port)
        self.teensy_found = False

        self.num_bytes_received

        self.end_code = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0]
        self.current_sensors = {}
        self.current_packet = []
        self.expected_size = 0

        self.settings_received = False
        self.sending_data = False

        # Variables needed for fake data
        self.time_begin = datetime.now()
        self.prev_time = datetime.now()
        self.prev_engine_val = 1800
        self.prev_lds_val = 0
        self.prev_br_lds_val = 0
        self.prev_bl_lds_val = 0
        self.prev_fr_lds_val = 0

        self.temp_data = {}
        for sensor in data.get_sensors(is_external=True, is_derived=False):
            self.temp_data[sensor] = {'value': None, 'has_been_updated': False, 'is_used': False}

        self.update()

    def update(self):
        if self.use_fake_inputs:
            self.check_connected_fake()
        # TODO implement actual serial reading
        if self.teensy_found:
            if self.teensy_ser.is_open():
                self.read_packet()
                self.send_packet()
            else:
                self.teensy_ser.open()
                self.update() 
        else:
            if self.teensy_port:
                print("Teensy found on port %s"%self.teensy_port)
                self.teensy_found = True
            else:
                print("No compatible Teensy found.")
                self.update()
    
    def get_Teensy_port(self):
        # Teensy USB serial microcontroller program id data:
        vendor_id = "16C0"
        product_id = "0483"
        serial_number = "12345"

        for port in list(list_ports.comports()):
            if port[2] == "USB VID:PID=%s:%s SNR=%s"%(vendor_id, product_id, serial_number):
                return port[0]
       
    # def is_connected(self):
        
    def read_packet(self):
        if self.use_fake_inputs:
            self.read_data_fake()
        
        # TODO implement actual serial reading
        while self.teensy_ser.is_open:
            self.current_packet.append(self.teensy_ser.read(1))
        
        if len(self.current_packet) >= 8:
            is_end = 1
            packet_size = len(self.current_packet)
            for i in packet_size:
                if i < 8 and is_end == 1:
                    if self.current_packet[packet_size - 8 + i] != self.end_code[i]:
                        not_end = 0
                else:
                    break
            if is_end == 1:
                self.unpacketize()
                self.current_packet.clear()

    def send_packet(self):
        self.teensy_ser.write(self.packetize())

    def packetize(self):
        if self.sending_data == False:
            # if 0x00, then parse settings and send settings
            # if 0x02, then parse data but send settings
            return (0x00 << 64 | 0xfffffffffffffff0)

        else:
            # if 0x01, then parse settings and send data
            # if 0x03, then parse data and send data
            return (0x01 << 64 | 0xfffffffffffffff0)

    def unpacketize(self):
        ack_code = self.current_packet[0]
        if ack_code > 0x03:
            return
        is_settings = not(ack_code and 0x02)
        while self.teensy_ser.is_open:
            if self.settings_received == False:
                """ if ack_code == 0x00 or ack_code == 0x01: """
                # if 0x00, then parse settings and send settings
                # if 0x01, then parse settings and send data
                self.sending_data = (ack_code == 0x01)
                self.settings_received = True
                self.current_sensors.clear()
                packet_size = len(self.current_packet)
                print("Received settings of length: ", str(packet_size))
                
                i = 0
                while i < packet_size - 8:
                    this_sensor_size = self.current_packet[i + 2]
                    this_sensor_id = ((self.current_packet[i + 1]) << 8) | self.current_packet[i]
                    self.current_sensors[this_sensor_id, this_sensor_size]
                    self.data.set_connected(this_sensor_id)
                    self.expected_size += this_sensor_size
                    i += 3
                
            else:
                # if 0x02, then parse data but send settings
                # if 0x03, then parse data and send data
                self.sending_data = (ack_code == 0x03)
                if len(self.current_packet) == self.expected_size:
                    i = 0
                    for key in self.current_sensors:
                        for size in range(0, self.current_sensors[key]):
                            this_data_value = ((this_data_value |
                                (self.current_packet[i + self.current_sensors[key] - size])) <<
                                    ((self.current_sensors[key] - size - 1) * 8))
                        self.data.add_value(this_data_value, key)
                        i += self.current_sensors[key]
                else:
                    self.settings_received = False

    # def attach_output_sensor(self):

    def check_connected_fake(self):
        self.data.set_connected("time")
        self.start_time = datetime.now()
        if (datetime.now() - self.time_begin).total_seconds() > 0:
            self.data.is_connected = True
            self.data.set_connected("engine_rpm")
            self.data.set_connected("secondary_rpm")
            self.data.set_connected("fl_lds")
            self.data.set_connected("br_lds")
            self.data.set_connected("fr_lds")
            self.data.set_connected("bl_lds")

        if (datetime.now() - self.time_begin).total_seconds() > 6:
            self.data.set_disconnected("fl_lds")
            self.data.set_disconnected("br_lds")
            self.data.set_disconnected("fr_lds")
            self.data.set_disconnected("bl_lds")

    def read_data_fake(self):
        with self.lock:
            if not self.data.is_connected:
                self.check_connected_fake()
            else:
                if (datetime.now() - self.prev_time).total_seconds() * 1000 > 5:
                    time = (datetime.now() - self.start_time).total_seconds()

                    self.data.add_value("time", time)

                    self.prev_engine_val = max(1800, min(4000, self.prev_engine_val + 0.5 * math.sin(
                        time * 10) + random.randrange(-2, 2)))
                    self.data.add_value('engine_rpm', self.prev_engine_val)

                    # temp_t = time%8-4
                    temp_t = time%6-4
                    self.prev_secondary_rpm_val = abs(2*temp_t*math.exp(temp_t)
                                                        - math.exp(1.65*temp_t))
                    self.data.add_value('secondary_rpm', self.prev_secondary_rpm_val)

                    self.prev_lds_val = 100 + 100 * math.sin(time / 20)
                    self.data.add_value('fl_lds', self.prev_lds_val)

                    self.prev_br_lds_val = max(1800, min(4000, self.prev_engine_val + 0.5 * math.sin(
                        time * 10) + random.randrange(-2, 2)))
                    self.data.add_value('br_lds', self.prev_br_lds_val)

                    self.prev_bl_lds_val = 100 + 100 * math.sin(time * 2) \
                                            + 100 * math.sin(time * 3.2)\
                                            + 100 * math.sin(time * 5.5)\
                                            + 100 * math.sin(time * 11.4)
                    self.data.add_value('bl_lds', self.prev_bl_lds_val)

                    self.prev_fr_lds_val = math.e**(3*math.sin(5*time))*abs(math.cos(5*time))+\
                                           50*math.cos(time)\
                                           +25*math.cos(1/4*time-math.pi/6)\
                                           +20*math.cos(1/25*time-math.pi/3)\
                                           +100+random.randrange(-2, 2)
                    self.data.add_value('fr_lds', self.prev_fr_lds_val)

