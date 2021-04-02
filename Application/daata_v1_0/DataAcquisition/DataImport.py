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

    def update(self):
        if self.use_fake_inputs:
            self.check_connected_fake()
        # TODO implement actual serial reading
        TeensyPort = getTeensyPort()
        TeensySer = serial.Serial(port=TeensyPort)
        if TeensyPort:
            print("Teensy found on port %s"%TeensyPort)
            while TeensySer.is_open:
               
                
                ackCode = TeensySer.read(1)
                #if 0x00, then parse settings and send settings
                if ackCode == 0x00:

                #if 0x01, then parse settings and send data
                else if ackCode == 0x01:
                    
                #f 0x02, then parse data but send settings
                else if ackCode == 0x02:
                    
                #if 0x03, then parse data and send data
                else if ackCode == 0x03:

                else:

                
        else:
            print("No compatible Teensy found.")


    def findEndCode(self):
        endCode = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0]
        endCodeProbe = list()
                tempByte = TeensySer.read(1)
                while (endCodeProbe != endCode):
                    if tempByte == 0xff:
                        endCodeProbe = list(TeensySer.read(8))
                        break
                    tempByte = TeensySer.read(1)
    
    def getTeensyPort(self):
        # Teensy USB serial microcontroller program id data:
        VENDOR_ID = "16C0"
        PRODUCT_ID = "0483"
        SERIAL_NUMBER = "12345"

        for port in list(list_ports.comports()):
            if port[2] == "USB VID:PID=%s:%s SNR=%s"%(VENDOR_ID, PRODUCT_ID, SERIAL_NUMBER):
                return port[0]
       

    def is_connected(self):
        
    def read_packet(self):
        if self.use_fake_inputs:
            self.read_data_fake()
        # TODO implement actual serial reading

    def send_packet(self):

    def packetize(self):

    def unpacketize(self):

    def attach_output_sensor(self):



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

