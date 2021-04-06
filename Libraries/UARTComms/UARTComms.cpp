#ifndef UARTCOMMS_CPP
#define UARTCOMMS_CPP

#include <Arduino.h>
#include <vector>

#include "UARTComms.h"
#include "SensorList.h"
#include <Sensor.h>


/*
 * Constructor
 */
UARTComms::UARTComms(uint16_t baud, uint8_t port_num) : _baud(baud), _port_num(port_num){
    _sending_period_us = 5000;

    switch (_port_num) {
        case 0: _port = &Serial;  break;
        case 1: _port = &Serial1; break;
        case 2: _port = &Serial2; break;
        case 3: _port = &Serial3; break;
        case 4: _port = &Serial4; break;
        case 5: _port = &Serial5; break;
        case 6: _port = &Serial6; break;
        case 7: _port = &Serial7; break;
        #if defined(__IMXRT1052__)   // Only defined for Teensy 4.1
        case 8: _port = &Serial8; break;
        #endif
        default: break;
    }

    _has_new_send_settings = true;
}

/*
 * @brief Initializes the hardware for the Serial Port
 */
void UARTComms::begin(){
    _port->begin(_baud);
}

/*
 * @brief Updates the Comm Port. Needs to be called at a speed >> 1/sending_period_us
 */
void UARTComms::update(){
    read_packet();
    send_packet();
}

void UARTComms::read_packet() {
    while(_port->available()){
        _data_receive.push_back(_port.read());
    }
    if(_data_receive.size() >= 8) {
        while(auto it = _data_receive.end() - 8; ){
            // Fill in later to check last 8 bytes against the end code.
        }
    }
    if(/*end of packet*/){
        unpacketize();
    }
}

void UARTComms::unpacketize() {
    // check ack byte --> [000000] & is_sending_data & is_receiving_data
    // ^ this is with respect to the sender.
    // if 0x03, then parse data     and send data.
    // if 0x02, then parse data     but send settings.
    // if 0x01, then parse settings and send data.
    // if 0x00, then parse settings and send settings.

    // ^ those will activate flags inside object.
    
    // on receive settings, rebuild _received_sensors from scratch.
    // make sure to connect to existing _input_sensors, otherwise GenericSensor.
    // WILL BE ORDERED!!

    // on receive data, check that length of packet is good
    // populate data into sensors using unpack(byte*)
}

void UARTComms::send_packet() {
    // if time_interval < passed time, continue

    // if need to send settings:
    // settings will be a list of id/byte_length pair.
    // ack is 0x01 or 0x00, depending on checks from unpacketize.
    // 2 byte id, 1 byte byte_length

    // if need to send data:
    // ack is 0x03 or 0x02, depending on checks from unpacketize.
    // packetize
    // Serial write that bad boi (dont forget end code)
    Serial.write(_data_send.data(), _data_send.size());
}

void UARTComms::packetize() {

}


/*
 * @brief Attaches a non-GenericSensor to recieve data from to be used in the main code
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void UARTComms::attach_input_sensor(Sensor &sensor, sensor_id_t id) {
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _input_sensors.begin(); it != _input_sensors.end(); it++){
        if(it->get_id() == id)
            return;
    }
    // Attach the sensor
    sensor.set_id(id);
    _input_sensors.push_back(&sensor);
    // Check to see if the sensor ID is in received sensors, if so, replace generic sensor with actual sensor
    for (auto it = _received_sensors.begin(); it != _received_sensors.end(); it++){
        if(it->get_id() == id) {
            Sensor *old_sensor = it;
            it = &sensor;
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
void UARTComms::attach_output_sensor(Sensor &sensor, sensor_id_t id){
    // Check to see if it is detached. If so, remove it from the detached list
    for (auto it = _detached_sensors.begin(); it != _detached_sensors.end(); it++){
        if(it->get_id() == id)
            _detached_sensors.erase(it);
    }
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _transmit_sensors.begin(); it != _transmit_sensors.end(); it++){
        if(it->get_id() == id)
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
        if(it->get_port_num() == through_comms->get_port_num())
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
        if(it->get_id() == id)
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


#endif