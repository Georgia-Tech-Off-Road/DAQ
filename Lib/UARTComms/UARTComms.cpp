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
void UARTComms::Begin(){
    _port->begin(_baud);
}

/*
 * @brief Updates the Comm Port. Needs to be called at a speed >> 1/sending_period_us
 */
void UARTComms::Update(){
    ReadIncomingData();
    if(CheckSendData()){
        SendData();
    }
}

/*
 * @brief Attaches a non-GenericSensor to recieve data from to be used in the main code
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void UARTComms::AttachInputSensor(Sensor *sensor, sensor_id_t id) {
    sensor->set_id(id);
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _input_sensors.begin(); it != _input_sensors.end(); it++){
        if(it->get_id() == id)
            return;
    }
    // Attach the sensor
    _input_sensors.push_back(sensor);
    // Check to see if the sensor ID is in received sensors, if so, replace generic sensor with actual sensor
    for (int i = 0; i < _received_sensors.size(); i++){
        if(_received_sensors[i]->get_id() == id)
            _received_sensors[i] = sensor;
    }
}

/*
 * @brief Attaches a non-GenericSensor to send data through the comm port
 * 
 * @param sensor A pointer to a sensor object (derived from Sensor class)
 * @param id The ID of the sensor being attached from SensorList
 */
void UARTComms::AttachOutputSensor(Sensor *sensor, sensor_id_t id){
    sensor->set_id(id);
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _output_sensors.begin(); it != _output_sensors.end(); it++){
        if(it->get_id() == id)
            return;
    }
    // Check to see if it is detached. If so, remove it from the detached list
    for (int i = 0; i < _detached_sensors.size(); i++){
        if(_detached_sensors[i] == id)
            _detached_sensors.erase(i);
    }
    // Attach the sensor
    _output_sensors.push_back(sensor);
    _has_new_send_settings = true;
}

/*
 * @brief Attaches a UARTComms object. It will send all its recieved data to be sent out
 *        out of the ports attached to it.
 * 
 * @param through_comms A pointer to a UARTComms object to sent its data to
 */
void AttachThroughputUart(UARTComms *through_comms){
    // Check to see if it is already attached. If so, do nothing
    for (auto it = _attached_uart_comms.begin(); it != _attached_uart_comms.end(); it++){
        if(it->get_port_num() == through_comms->get_port_num())
            return;
    }
    // Attach the comm port
    _attatched_uart_comms.push_back(through_comms);
    UpdateThroughputUarts();
}

/*
 * @brief Detaches a sensor so it won't send it's data out of this port
 * 
 * @param id The ID of the sensor being attached from SensorList
 */
void DetachOutputSensor(sensor_id_t id){
    // Check to see if it is already detached. If so, do nothing
    for (int i = 0; i < _detached_sensors.size(); i++){
        if(_detached_sensors[i] == id)
            return;
    }
    // Check to see if it is already attached. If so, remove it
    for (int i = 0; i < _attached_sensors.size(); i++){
        if(_attached_sensors[i]->get_id() == id){
            _attached_sensors.erase(i);
            _has_new_send_settings = true;
        }
            
    }

}

#endif