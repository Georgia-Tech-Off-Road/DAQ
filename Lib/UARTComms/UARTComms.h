#ifndef UARTCOMMS_H
#define UARTCOMMS_H

#include <Arduino.h>
#include <Sensor.h>
#include <vector>

class UARTComms {
private:
    HardwareSerial * const _port;
    const uint32_t _baud;

    const uint16_t _sending_period_us;
    uint32_t _time_at_last_receive;
    uint32_t _time_at_last_send;

    bool _is_sending_data;
    bool _is_receiving_data;
    std::vector<byte> _packet_receive;
    std::vector<byte> _packet_send;
    static constexpr byte _end_code[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0};

    std::vector<BaseSensor*> _input_sensors;
    std::vector<BaseSensor*> _received_sensors;
    std::vector<BaseSensor*> _transmit_sensors;
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

    HardwareSerial *get_port();
};

#endif