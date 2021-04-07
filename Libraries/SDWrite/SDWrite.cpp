#ifndef SDWRITE_CPP
#define SDWRITE_CPP

#include <Arduino.h>

#include "SDWrite.h"

/*
 * Constructor
 */
SDWrite::SDWrite(uint8_t port) : 
    _port(port),
    _sending_period_us(10000),
    _time_at_last_send(0) { }

/*
 * @brief Initializes the hardware for the Serial Port
 */
void SDWrite::begin(const char* filename){
    SD.begin(_port);
    _filename = String(filename);
}

/*
 * @brief Updates the Comm Port. Needs to be called at a speed >> 1/sending_period_us
 */
void SDWrite::update(){
    send_packet();
}

void SDWrite::send_packet() {
    // if time_interval < passed time, continue
    uint32_t time_current = micros();
    if(abs(time_current - _time_at_last_send) >= _sending_period_us){
        packetize();
        File datafile = SD.open(_filename.c_str(), FILE_WRITE);
        datafile.write(_packet_send.data(), _packet_send.size());
        datafile.close();
        // Serial.println("Wrote data");
        _time_at_last_send = time_current;
        _packet_send.clear();
    }
}

void SDWrite::packetize() {
    byte ack = (0x02 & (_is_sending_data << 1)) | (0x01 & (0));
    _packet_send.push_back(ack);
    if(_is_sending_data){
        // ack should be 0x02
        for(auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
            byte *pack = new byte[(*it)->get_pack_bytes()];
            (*it)->pack(pack);
            for(int i = 0; i < (*it)->get_pack_bytes(); ++i) _packet_send.push_back(pack[i]);
            delete pack;
        }
        for(auto tp = _throughput_uart_comms.begin(); tp != _throughput_uart_comms.end(); tp++){
            for(auto it = (*tp)->_received_sensors.begin(); it != (*tp)->_received_sensors.end(); it++){
                sensor_id_t id = (*it)->get_id();
                bool detached = 0;
                for(auto ds = _detached_sensors.begin(); (ds != _detached_sensors.end() && !detached); ds++){
                    if((*ds) == id) detached = 1;
                }
                if(!detached){
                    byte *pack = new byte[(*it)->get_pack_bytes()];
                    (*it)->pack(pack);
                    for(int i = 0; i < (*it)->get_pack_bytes(); ++i) _packet_send.push_back(pack[i]);
                    delete pack;
                }
            }
        }
    } else {
        // ack should be 0x00
        for(auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
            uint16_t id = (*it)->get_id();
            byte *id_ptr = (byte*) &id;
            byte pack_bytes = (*it)->get_pack_bytes();
            _packet_send.push_back(*id_ptr);
            _packet_send.push_back(*(id_ptr + 1));
            _packet_send.push_back(pack_bytes);
        }
        for(auto tp = _throughput_uart_comms.begin(); tp != _throughput_uart_comms.end(); tp++){
            for(auto it = (*tp)->_received_sensors.begin(); it != (*tp)->_received_sensors.end(); it++){
                bool detached = 0;
                uint16_t id = (*it)->get_id();
                for(auto ds = _detached_sensors.begin(); (ds != _detached_sensors.end() && !detached); ds++){
                    if((*ds) == id) detached = 1;
                }
                if(!detached){
                    byte id_msb = (byte) id >> 8;
                    byte id_lsb = (byte) id;
                    byte pack_bytes = (*it)->get_pack_bytes();
                    _packet_send.push_back(id_lsb);
                    _packet_send.push_back(id_msb);
                    _packet_send.push_back(pack_bytes);
                }
            }
        }
        _is_sending_data = 1;
    }
    for(int i = 0; i < 8; ++i) _packet_send.push_back(_end_code[i]);

    // if need to send settings:
    // settings will be a list of id/byte_length pair.
    // ack is 0x01 or 0x00, depending on checks from unpacketize.
    // 2 byte id, 1 byte byte_length

    // if need to send data:
    // ack is 0x03 or 0x02, depending on checks from unpacketize.
    // packetize
}

uint8_t SDWrite::get_expected_transmit_bytes() {
    uint8_t bytes = 0;
    for(auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++) bytes += (*it)->get_pack_bytes();
    for(auto tp = _throughput_uart_comms.begin(); tp != _throughput_uart_comms.end(); tp++){
        for(auto it = (*tp)->_received_sensors.begin(); it != (*tp)->_received_sensors.end(); it++){
            bool detached = 0;
            uint16_t id = (*it)->get_id();
            for(auto ds = _detached_sensors.begin(); (ds != _detached_sensors.end() && !detached); ds++){
                if((*ds) == id) detached = 1;
            }
            if(!detached) bytes += (*it)->get_pack_bytes();
        }
    }

    return (9 + bytes);
}

/*
 * @brief Attaches a non-GenericSensor to send data through the comm port
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void SDWrite::attach_output_sensor(BaseSensor &sensor, sensor_id_t id){
    // Check to see if it is detached. If so, remove it from the detached list
    for (auto ds = _detached_sensors.begin(); ds != _detached_sensors.end(); ds++){
        if((*ds) == id)
            _detached_sensors.erase(ds);
    }
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
        if((*it)->get_id() == id)
            return;
    }
    // Attach the sensor
    sensor.set_id(id);
    _transmit_sensors.push_back(&sensor);
}


/*
 * @brief Attaches a UARTComms object. It will send all its recieved data to be sent out
 *        out of the ports attached to it.
 * 
 * @param through_comms A pointer to a UARTComms object to sent its data to
 */
void SDWrite::attach_throughput_uart(UARTComms &through_comms){
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _throughput_uart_comms.begin(); it != _throughput_uart_comms.end(); it++){
        if((*it)->get_port() == through_comms.get_port())
            return;
    }
    // Attach the comm port
    _throughput_uart_comms.push_back(&through_comms);
}

/*
 * @brief Detaches a sensor so it won't send it's data out of this port
 * 
 * @param id The ID of the sensor being attached from SensorList
 */
void SDWrite::detach_output_sensor(sensor_id_t id){
    // Check to see if it is already attached. If so, remove it
    for (auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
        if((*it)->get_id() == id)
            _transmit_sensors.erase(it);
    }
    // Check to see if it is already detached. If so, do nothing
    for (auto it = _detached_sensors.begin(); it != _detached_sensors.end(); it++){
        if(*it == id)
            return;
    }
    // Detach the sensor
    _detached_sensors.push_back(id);
}

const uint8_t SDWrite::get_port() {
    return _port;
}

#endif