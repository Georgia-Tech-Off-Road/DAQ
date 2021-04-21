import sys
import serial
from serial.tools import list_ports
import time
import logging
from datetime import datetime
import random
import math

from DataAcquisition.Data import Data
from DataAcquisition.SensorId import SensorId

logger = logging.getLogger("DataAcquisition")

class DataImport:
    def __init__(self, data, lock, is_data_collecting, use_fake_inputs):
        self.data = data
        self.lock = lock
        self.use_fake_inputs = use_fake_inputs
        self.is_data_collecting = is_data_collecting
        self.start_time = datetime.now()
        
        # Manually change the COM port below to the correct
        # port that the teensy appears on your device manager
        try:
            self.teensy_port = self.get_Teensy_port()
            self.teensy_ser = serial.Serial()
            self.teensy_found = True
        except Exception as e:
            logger.error(e)

        # self.num_bytes_received

        self.end_code = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0]
        self.current_sensors = {}
        self.current_packet = []
        self.packet_index = 0
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

    def check_connected(self):
        return self.teensy_ser.is_open

    def update(self):
        """
        if self.use_fake_inputs:
            self.check_connected_fake()
        """
        # TODO implement actual serial reading
        
        if self.teensy_found:
            self.teensy_ser.open()
            if self.teensy_ser.is_open:
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
    
    def get_Teensy_port(self):
        # Teensy USB serial microcontroller program id data:
        vendor_id = "16C0"
        product_id = "0483"
        serial_number = "12345"

        for port in list(list_ports.comports()):
            if port[2] == "USB VID:PID=%s:%s SNR=%s"%(vendor_id, product_id, serial_number):
                return port[0]
       
    # def is_connected(self):

    def read_serial(self):
        while self.teensy_ser.is_open and (self.teensy_ser.in_waiting() > 0):
            self.current_packet.append(self.teensy_ser.read(1))

    def find_encode(self):
        if (len(self.current_packet) - self.packet_index) < (self.expected_size + 8):
            self.read_serial()
        tempIndex = self.packet_index
        tempBytes = self.current_packet[(tempIndex) : (tempIndex + 8)]
        while tempBytes != self.end_code:
            tempIndex += 1
        tempIndex += 8
        return tempIndex

    def read_packet(self):
        """
        if self.use_fake_inputs:
            self.read_data_fake()
        """
        # TODO implement actual serial reading
        
        # self.read_serial()
        # self.find_encode()

        self.unpacketize()

    def send_packet(self):
        self.teensy_ser.write(self.packetize())

    def packetize(self):
        ack_0 = b'\x00\xff\xff\xff\xff\xff\xff\xff\xf0'
        ack_1 = b'\x01\xff\xff\xff\xff\xff\xff\xff\xf0'
        ack_2 = b'\x02\xff\xff\xff\xff\xff\xff\xff\xf0'
        ack_3 = b'\x03\xff\xff\xff\xff\xff\xff\xff\xf0'
        
        if self.settings_received == False:
            # if 0x00, then parse settings and send settings
            # if 0x02, then parse data but send settings
            return ack_0

        else:
            # if 0x01, then parse settings and send data
            # if 0x03, then parse data and send data
            return ack_3

    def unpacketize(self):
        self.read_serial()
        self.packet_index = self.find_encode()
        ack_code = self.current_packet[self.packet_index]
        self.sending_data = ((ack_code & 0x01) == 0x01)
        self.packet_index += 1
        
        if ack_code > 0x03 or ack_code < 0x00:
            self.packet_index = self.find_encode()
            ack_code = self.current_packet[self.packet_index]
            self.sending_data = ((ack_code & 0x01) == 0x01)
            self.packet_index += 1

        while (len(self.current_packet) - self.packet_index) > (self.expected_size + 8):
            if (ack_code == 0x00 or ack_code == 0x01) and self.settings_received == False:
                # if 0x00, then parse settings and send settings
                # if 0x01, then parse settings and send data
                
                self.settings_received = True
                # packet_size = len(self.current_packet)
                
                if not(self.current_sensors == None):
                    for key in self.current_sensors.keys():
                        self.data.set_disconnected(key)
                
                self.current_sensors.clear()
                
                tempBytes = self.current_packet[(self.packet_index) : (self.packet_index + 8)]
                while not(tempBytes == self.end_code):
                    this_sensor_id = (((self.current_packet[self.packet_index + 1]) << 8) 
                        | (self.current_packet[self.packet_index]))
                    this_sensor_size = self.current_packet[self.packet_index + 2]
                    self.current_sensors[this_sensor_id] = this_sensor_size
                    self.data.set_connected(this_sensor_id)
                    self.expected_size += this_sensor_size
                    self.packet_index += 3
                    tempBytes = self.current_packet[(self.packet_index) : (self.packet_index + 8)]
                
                print("Received settings of length: ", str(self.expected_size))
                self.packet_index += 8
            
            elif (ack_code == 0x02 or ack_code == 0x03) and self.settings_received:
                # if 0x02, then parse data but send settings
                # if 0x03, then parse data and send data
                
                if (self.find_encode - self.packet_index - 8) == self.expected_size:
                    for key in self.current_sensors:
                        this_data_value = 0
                        if isinstance(SensorId[key]["num_bytes"], list):
                            valueList = []
                            running_size = 0
                            for values in range(len(SensorId[key]["num_bytes"])):
                                entry_size = SensorId[key]["num_bytes"][values]
                                for valueSize in range(entry_size):
                                    this_data_value = ((this_data_value |
                                        (self.current_packet[self.packet_index + entry_size - valueSize + (running_size)]))
                                        << ((entry_size - valueSize - 1) * 8))
                                valueList.append(this_data_value)
                                running_size += entry_size
                            self.data.add_value(valueList, key)
                            self.packet_index += self.current_sensors[key]
                        else:
                            for size in range(self.current_sensors[key]):
                                this_data_value = ((this_data_value |
                                    (self.current_packet[self.packet_index + self.current_sensors[key] - size]))
                                    << ((self.current_sensors[key] - size - 1) * 8))
                            self.data.add_value(this_data_value, key)
                            self.packet_index += self.current_sensors[key]
                    print("Received some data")
                else:
                    self.settings_received = False
            else:
                self.find_encode()

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

