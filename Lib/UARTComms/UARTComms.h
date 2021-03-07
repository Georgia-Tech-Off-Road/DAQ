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

public:
    void attach_input_sensor    (Sensor *sensor, SensorInfo *info);
    void attach_output_sensor   (Sensor *sensor);
    void attach_throughput_uart (UARTComms *comms, std::vector<SensorInfo> ignore);


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