#ifndef SDWRITE_H
#define SDWRITE_H

#include <Arduino.h>
#include <Sensor.h>
#include <UARTComms.h>
#include <SD.h>
#include <vector>

class SDWrite {
private:
    const uint8_t _port;

    String _filename;

    const uint16_t _sending_period_us;
    uint32_t _time_at_last_send;

    bool _is_sending_data;
    std::vector<byte> _packet_send;
    const end_code_t _end_code;

    std::vector<BaseSensor*> _transmit_sensors;
    std::vector<sensor_id_t> _detached_sensors;
    std::vector<UARTComms*> _throughput_uart_comms;

    void send_packet();
    void packetize();
    
public:
    SDWrite(uint8_t port);

    void update();
    void begin(const char* filename);

    void attach_output_sensor(BaseSensor &sensor, sensor_id_t id);

    void attach_throughput_uart(UARTComms &through_comms);
    void detach_output_sensor(sensor_id_t id);

    const uint8_t get_port();

    uint8_t get_expected_transmit_bytes();
};


#endif