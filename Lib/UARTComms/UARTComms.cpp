#ifndef UARTCOMMS_CPP
#define UARTCOMMS_CPP

#include "UARTComms.h"



void UARTComms::attach_input_sensor(Sensor *sensor) {
    _input_sensors.push_back(sensor);
}

#endif