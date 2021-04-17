#ifndef UARTCOMMS_CPP
#define UARTCOMMS_CPP

#include <Arduino.h>

#include "UARTComms.h"

/*
 * Constructor
 */
UARTComms::UARTComms(uint32_t baud, HardwareSerial &hardware_port) : 
    _baud(baud), 
    _hardware_port(&hardware_port),
    _port(&hardware_port),
    _is_hardware_port(1),
    _sending_period_us(10000),
    _time_at_last_send(0),
    _time_at_last_receive(0),
    _is_sending_data(0),
    _is_receiving_data(0) { }

/*
 * Constructor
 */
UARTComms::UARTComms(uint32_t baud, usb_serial_class &usb_port) : 
    _baud(baud), 
    _usb_port(&usb_port),
    _port(&usb_port),
    _is_hardware_port(0),
    _sending_period_us(10000),
    _time_at_last_send(0),
    _time_at_last_receive(0),
    _is_sending_data(0),
    _is_receiving_data(0) { }

/*
 * @brief Initializes the hardware for the Serial Port
 */
void UARTComms::begin(){
    if(_is_hardware_port){
        _hardware_port->begin(_baud);
    } else {
        _usb_port->begin(_baud);
    }
}

/*
 * @brief Updates the Comm Port. Needs to be called at a speed >> 1/sending_period_us
 */
void UARTComms::update(){
    read_packet();
    send_packet();
}

void UARTComms::read_packet() {
    // RESET _IS_RECEIVING_DATA WHEN TIME SINCE LAST BYTE RECEIVED IS LARGE
    if(_port->available()){
        // Serial.print("p");
        while(_port->available()) _packet_receive.push_back(_port->read());
        _time_at_last_receive = micros();

        if(_packet_receive.size() >= 8) {
            bool is_end_of_packet = 1;
            int packet_size = _packet_receive.size();
            for (int i = 0; (i < 8 && is_end_of_packet); ++i){
                if(_packet_receive[packet_size - 8 + i] != _end_code[i]) is_end_of_packet = 0;
            }
            if(is_end_of_packet){
                unpacketize();
                _packet_receive.clear();
            }
        }
    } else if ((micros() - _time_at_last_receive) > _sending_period_us * 4) {
        _is_receiving_data = 0;
    }
}

void UARTComms::unpacketize() {
    // check ack byte --> [000000] & is_sending_data & is_receiving_data
    // ^ this is with respect to the sender.
    // if 0x03, then parse data     and send data.
    // if 0x02, then parse data     but send settings.
    // if 0x01, then parse settings and send data.
    // if 0x00, then parse settings and send settings.

    const byte ack = _packet_receive[0];
    if(ack > 0x03) return; // ACK IS NOT STANDARD! CHANGE TO DO SOMETHING ELSE

    const bool sender_is_sending_data   = ack & 0x02; 
    const bool sender_is_receiving_data = ack & 0x01;

    _is_sending_data = sender_is_receiving_data;

    if(sender_is_sending_data) { // RECEIVING DATA
        if(get_expected_receive_bytes() == _packet_receive.size()) { // packet lengths good
            // Serial.print("\nreceived data of length: ");
            // Serial.println(_packet_receive.size());
            const byte *packet_loc = _packet_receive.data() + 1; // pointer to constant data, +1 to skip ack
            for(auto it = _received_sensors.begin(); it != _received_sensors.end(); it++){
                // Serial.println("sensor");
                (*it)->unpack(packet_loc);
                    // Serial.println(*((uint32_t*)packet_loc));
                // Serial.print("location inside uartcomms: ");
                // Serial.println((uint32_t)(*it)->get_id());
                packet_loc += (*it)->get_pack_bytes();
            }
        } else { // packet lengths not good
            // CHANGE LOGIC TO DO THIS AFTER X AMOUNTS OF MISSES.
            _is_receiving_data = 0;
        }
    } else { // RECEIVING SETTINGS
        //Serial.print("\nreceived settings of length: ");
        //Serial.println(_packet_receive.size());

        for(auto rs = _received_sensors.begin(); rs != _received_sensors.end(); rs++){
            bool match = 0;
            for(auto is = _input_sensors.begin(); (is != _input_sensors.end() && !match); is++){
                if((*rs)->get_id() == (*is)->get_id()) match = 1;
            }
            if(!match) delete (*rs);
        }

        _received_sensors.clear();
        const byte *packet_loc = _packet_receive.data() + 1; // pointer to constant data, +1 to skip ack
        // const byte * const packet_end = _packet_receive.data() - 8;
        const byte * const packet_end = _packet_receive.data() + _packet_receive.size() - 9;
        // Serial.println((uint32_t)packet_loc, HEX);
        // Serial.println((uint32_t)packet_end, HEX);
        while (packet_loc < packet_end) {
            // uint8_t id = *((uint16_t *) packet_loc);
            uint8_t id_lsb = *((uint8_t *) packet_loc);
            uint8_t id_msb = *((uint8_t *) (packet_loc + 1));
            uint16_t id = ((uint16_t) id_msb << 8) | (uint16_t) id_lsb;
            uint8_t pack_bytes = *((uint8_t *) (packet_loc + 2));

            // Check if Sensor is in list of input sensors.
            BaseSensor *existing_input_sensor = 0;
            for (auto it = _input_sensors.begin(); it != _input_sensors.end(); it++){
                if((*it)->get_id() == id) existing_input_sensor = *it;
            }
            //Serial.print(existing_input_sensor != 0);
            if(existing_input_sensor != 0) _received_sensors.push_back(existing_input_sensor);
            else _received_sensors.push_back(new GenericSensor((sensor_id_t) id, pack_bytes));

            packet_loc += 3;
        }
        //Serial.print("created ");
        //Serial.print(_received_sensors.size());
        //Serial.println(" _received_sensors");
        _is_receiving_data = 1;
    }


    // ^ those will activate flags inside object.
    
    // on receive settings, rebuild _received_sensors from scratch.
    // make sure to connect to existing _input_sensors, otherwise GenericSensor.
    // WILL BE ORDERED!!

    // on receive data, check that length of packet is good
    // populate data into sensors using unpack(byte*)
}

void UARTComms::send_packet() {
    // if time_interval < passed time, continue
    uint32_t time_current = micros();
    if(abs(time_current - _time_at_last_send) >= _sending_period_us){
        packetize();
        _port->write(_packet_send.data(), _packet_send.size());
        _time_at_last_send = time_current;
        _packet_send.clear();
    }
}

void UARTComms::packetize() {
    byte ack = (0x02 & (_is_sending_data << 1)) | (0x01 & (_is_receiving_data));
    _packet_send.push_back(ack);
    if(_is_sending_data){
        // ack should be 0x03 or 0x02
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
        // ack should be 0x01 or 0x00
        for(auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
            uint16_t id = (*it)->get_id();
            // byte id_msb = *((byte *)(&id));
            byte id_msb = (byte) id >> 8;
            byte id_lsb = (byte) id;
            byte pack_bytes = (*it)->get_pack_bytes();
            _packet_send.push_back(id_lsb);
            _packet_send.push_back(id_msb);
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

uint8_t UARTComms::get_expected_receive_bytes() {
    uint8_t bytes = 0;
    for(auto it = _received_sensors.begin(); it != _received_sensors.end(); it++) bytes += (*it)->get_pack_bytes();
    return (9 + bytes);
}

uint8_t UARTComms::get_expected_transmit_bytes() {
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
 * @brief Attaches a non-GenericSensor to recieve data from to be used in the main code
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void UARTComms::attach_input_sensor(BaseSensor &sensor, sensor_id_t id) {
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _input_sensors.begin(); it != _input_sensors.end(); it++){
        if((*it)->get_id() == id)
            return;
    }
    // Attach the sensor
    sensor.set_id(id);
    sensor.set_type(PASSIVE);
    _input_sensors.push_back(&sensor);
    // Check to see if the sensor ID is in received sensors, if so, replace generic sensor with actual sensor
    for (auto it = _received_sensors.begin(); it != _received_sensors.end(); it++){
        if((*it)->get_id() == id) {
            BaseSensor *old_sensor = (*it);
            *it = &sensor;
            delete old_sensor;
        }
    }
}

/*
 * @brief Attaches a non-GenericSensor to send data through the comm port
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void UARTComms::attach_output_sensor(BaseSensor &sensor, sensor_id_t id){
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
void UARTComms::attach_throughput_uart(UARTComms &through_comms){
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
void UARTComms::detach_output_sensor(sensor_id_t id){
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

HardwareSerial *UARTComms::get_port() {
    return _port;
}

#endif