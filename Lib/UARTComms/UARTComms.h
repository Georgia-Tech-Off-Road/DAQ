#ifndef UARTCOMMS_H
#define UARTCOMMS_H

#include <Arduino.h>
#include <Sensor.h>
#include <vector>

#include "SensorList.h"

class UARTComms {
private:
    uint8_t _port_num;
    HardwareSerial *_port;
    uint32_t _baud;

    uint16_t _sending_period_us;
    uint32_t _time_at_last_send;

    bool _has_new_send_settings;
    bool _has_new_receive_settings;

    std::vector<byte> _temp_data_receive;
    static constexpr uint8_t _end_code[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0};

    std::vector<Sensor*> _input_sensors;
    std::vector<Sensor*> _received_sensors;
    std::vector<Sensor*> _transmit_sensors;
    std::vector<uint16_t> _detached_sensors;
    std::vector<UARTComms*> _attached_uart_comms;

public:
    UARTComms(uint32_t baud);

    void Update();
    void Begin();
    void ReadIncomingData();
    bool CheckSendData();
    void SendData();

    void AttachInputSensor(Sensor *sensor, sensor_id_t id);
    void AttachOutputSensor(Sensor *sensor, sensor_id_t id);
    void DetachOutputSensor(sensor_id_t id);

    void AttachThroughputUart(UARTComms *comms);
    void UpdateThroughputUarts();

    uint8_t get_total_transmit_bytes();
    uint8_t get_total_receive_bytes();

    uint8_t get_port_num();

    void Packetize();
    void Unpacketize();

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