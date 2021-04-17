#ifndef UARTCOMMS_H
#define UARTCOMMS_H

#include <Arduino.h>
#include <Sensor.h>
#include <vector>

class SDWrite;

struct end_code_t {
    byte code[8];
    end_code_t() {
        for(uint8_t i = 0; i < 7; ++i) code[i] = 0xFF;
        code[7] = 0xF0;
    }
    const byte& operator[] (uint8_t i) const {
        return code[i];
    }
};

class UARTComms {
private:
    HardwareSerial * const _hardware_port;
    usb_serial_class * const _usb_port;
    Stream * const _port;
    bool _is_hardware_port;
    const uint32_t _baud;

    const uint16_t _sending_period_us;
    uint32_t _time_at_last_receive;
    uint32_t _time_at_last_send;

    bool _is_sending_data;
    bool _is_receiving_data;
    std::vector<byte> _packet_receive;
    std::vector<byte> _packet_send;
    const end_code_t _end_code;

    std::vector<BaseSensor*> _input_sensors;
    std::vector<BaseSensor*> _received_sensors;
    std::vector<BaseSensor*> _transmit_sensors;
    std::vector<sensor_id_t> _detached_sensors;
    std::vector<UARTComms*> _throughput_uart_comms;

    void read_packet();
    void send_packet();
    void packetize();
    void unpacketize();

    friend class SDWrite;

    
public:
    UARTComms(uint32_t baud, HardwareSerial &hardware_port);
    UARTComms(uint32_t baud, usb_serial_class &usb_port);

    void update();
    void begin();

    void attach_input_sensor (BaseSensor &sensor, sensor_id_t id);
    void attach_output_sensor(BaseSensor &sensor, sensor_id_t id);

    void attach_throughput_uart(UARTComms &through_comms);
    void detach_output_sensor(sensor_id_t id);

    HardwareSerial *get_port();

    uint8_t get_expected_receive_bytes();
    uint8_t get_expected_transmit_bytes();
};

#endif