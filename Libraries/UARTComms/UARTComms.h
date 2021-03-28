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

    bool _is_receiving;
    std::vector<byte> _data_receive;
    std::vector<byte> _data_send;
    static constexpr byte _end_code[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0};

    std::vector<Sensor*> _input_sensors;
    std::vector<Sensor*> _received_sensors;
    std::vector<Sensor*> _transmit_sensors;
    std::vector<sensor_id_t> _detached_sensors;
    std::vector<UARTComms*> _throughput_uart_comms;

    void read_packet();
    void send_packet();
    void packetize();
    void unpacketize();

    uint8_t get_expected_transmit_bytes();
    uint8_t get_expected_receive_bytes();
    
public:
    UARTComms(uint16_t baud, uint8_t port_num);

    void update();
    void begin();

    void attach_input_sensor (Sensor &sensor, sensor_id_t id);
    void attach_output_sensor(Sensor &sensor, sensor_id_t id);

    void attach_throughput_uart(UARTComms &through_comms);
    void detach_output_sensor(sensor_id_t id);

    uint8_t get_port_num();
}

#endif