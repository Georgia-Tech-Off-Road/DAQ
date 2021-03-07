#ifndef UARTCOMMS_H
#define UARTCOMMS_H

#include <Arduino.h>
#include <Sensor.h>
#include <vector>

#include "SensorList.h"

class UARTComms {
private:
    HardwareSerial _port;
    uint16_t _baud;
    std::vector<Sensor*> _input_sensors;
    std::vector<Sensor*> _received_sensors;
    std::vector<Sensor*> _transmit_sensors;
    std::vector<Sensor*> _detatched_sensors;

public:
    void attach_input_sensor    (Sensor *sensor, sensor_id_t id);
    void attach_output_sensor   (Sensor *sensor, sensor_id_t id);
    // Appends GenericSensor(s) from through_comms's _received_sensors to this->_transmit_sensors
    void attach_throughput_uart (UARTComms *through_comms);
    void detatch_output_sensor  (sensor_id_t id);


    void receive_settings(){
        if(){ // Checks if sensor is in list of _input_sensors
            _received_sensors.push_back(_input_sensor->get(i));
            _input_sensor->set_connected();
        } else { // Otherwise make a GenericSensor
            _received_sensors.push_back(new GenericSensor());
        }
    }

}

#endif