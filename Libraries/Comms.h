#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

/*
 * This utility class is designed for robust communication using UART protocols and is designed to work mainly with Teensy
 * and Arduino but should work with any device supporting UART communication.
 * 
 * @TODO: Add support for Teensy < 4.0 and for Arduino
 */

class Comms {
private:
    Comms() {}
    static uint16_t _is_enabled;
    static uint16_t _baud_rate[9];
    static HardwareSerial *_serial_ports[9];
    static uint8_t _sensor_list[64][4];

    static uint8_t _address;
    static uint8_t _packet_type;
    static uint8_t _data[60];
    static uint8_t _data2[60]; //Used as a buffer for data coming from another Teensy and going to the PC
    const static uint32_t _end_code = 0xFFFFFFF0;
    static uint8_t _index;
    static uint8_t _index2;
    static bool _has_new_packet;
    static bool _has_requested_data;

    constexpr static uint8_t _pingArray[60] = {0};

public:

    /*
     * @brief Call these functions before the begin function to enable to respective Serial ports
     * 
     * @param serial_number The serial port which you wish to make active (must be a number from 0-8)
     * @param baud_rate The baud rate to be used with the respective serial port
     */
    static void enableSerial(uint8_t serial_number, uint16_t baud_rate) {
        _is_enabled |= (1<<serial_number);
        _baud_rate[serial_number] = baud_rate;
    }


    /*
     * @brief Necessary to set up hardware used for communication
     * 
     * @param address Teensy specific address used for the PC to know which Teensy it is communicating with
     */
    static void begin(){
        _serial_ports[0] = &Serial;
        _serial_ports[1] = &Serial1;
        _serial_ports[2] = &Serial2;
        _serial_ports[3] = &Serial3;
        _serial_ports[4] = &Serial4;
        _serial_ports[5] = &Serial5;
        _serial_ports[6] = &Serial6;
        _serial_ports[7] = &Serial7;
        #if defined(__IMXRT1052__) 
        _serial_ports[8] = &Serial8;
        #endif
        for(uint8_t i = 0; i <= 8; i++){
            if(_is_enabled & (1<<i)){
                _serial_ports[i]->begin(_baud_rate[i]);
            }
        }
        setup_sensor_list();
    }

    static void setup_sensor_list(uint8_t *active_sensors) {
        #define ENGINE_SPEED_RPM 0
        #define SECONDARY_SPEED_RPM 1
        #define FRONT_LEFT_SHOCK 2
        #define FRONT_RIGHT_SHOCK 3
        #define BACK_LEFT_SHOCK 4
        #define BACK_RIGHT_SHOCK 5
        #define ACCELEROMETER 6
        #define GYRO 7
        /* Sensor Setting Key
            0: sensor name
            1: enabled or disabled
            2: number of data bytes
        */

        _sensor_list[0][0] = ENGINE_SPEED_RPM;
        _sensor_list[0][1] = 1;
        _sensor_list[0][2] = 2;

        _sensor_list[1][0] = SECONDARY_SPEED_RPM;
        _sensor_list[1][1] = 0;
        _sensor_list[1][2] = 1;
        
        _sensor_list[2][0] = FRONT_LEFT_SHOCK;
        _sensor_list[2][1] = 1;
        _sensor_list[2][2] = 2;
    }

    /*
     * @brief Run this every loop to recieve any bytes in the serial buffer and parse them
     *  If data has been recieved on Serial1 or Serial it must be from PC. Data is either for Teensy or another Teensy further down the line
     *  If data has been recieved on Serial2 it must be going to PC. Buffer the data, check for completeness, then send out Serial 1
     */
    static void update(){
        for(uint8_t i = 0; i <= 8; i++){
            if(_is_enabled & (1<<i)){
                continue;
            }
            if(_serial_ports[i]->available()){

            }


        }

















        bool byteArrayFull = false;
        uint32_t temp_code = 0;
        if(Serial.available() || Serial1.available()){
            _data[_index] = Serial.available() ? Serial.read() : Serial1.read();
            if(_index >= 3) {
                temp_code = (uint32_t)_data[_index - 3] << 24 | (uint32_t)_data[_index - 2] << 16 | (uint32_t)_data[_index - 1] << 8 | (uint32_t)_data[_index];
                byteArrayFull = (temp_code == _end_code && _index == 59);
            }
            _index = (temp_code == _end_code  || _index == 59 || byteArrayFull) ? 0 : (_index + 1);
            if(byteArrayFull){
                //Data is for device
                if(_data[0] == _address){
                    _packet_type = _data[1];
                    if(_packet_type == 0){
                        transmitData(_pingArray, _address == 1 ? 0:1);
                    }
                    _has_new_packet = (_packet_type <= 10 && _packet_type >= 1);
                    _has_requested_data = (_packet_type >= 11);
                }
                //Data is being sent to another Teensy
                else{
                    transmitData(_data, 2);
                }
            }
        }
        if(Serial2.available()){
            _data2[_index2] = Serial2.read();
            if(_index2 >= 3) {
                temp_code = (uint32_t)_data2[_index2 - 3] << 24 | (uint32_t)_data2[_index2 - 2] << 16 | (uint32_t)_data2[_index2 - 1] << 8 | (uint32_t)_data2[_index2];
                byteArrayFull = (temp_code == _end_code && _index2 == 59);
            }
            _index2 = (temp_code == _end_code  || _index2 == 59 || byteArrayFull) ? 0 : (_index2 + 1);
            if(byteArrayFull){
                //Data is for PC
                transmitData(_data2, _address == 1 ? 0:1);
            }
        }
    }
    
    /*
     * @brief Checks to see if there is a new packet and resets flag
     * 
     * @return the state of the member variable _has_new_packet
     */
    static bool hasNewPacket(){
        if(_has_new_packet){
            _has_new_packet = false;
            return true;
        }
        return false;         
    }

    /*
     * @brief Checks to see if data has been requested and resets flag
     * 
     * @return the state of the member variable _has_requested_data
     */
    static bool hasRequestedData(){
        if(_has_requested_data){
            _has_requested_data = false;
            return true;
        }
        return false;         
    }

    /*
     * @brief Returns the packet type of the most recent set of data from the PC
     * 
     * @return the state of the member variable _packet_type
     */
    static uint8_t getPacketType(){ return _packet_type; }

    /*
     * @brief Fills the passed in array data with the recieved contents of _data
     * 
     * @param data A pointer to the array of data being filled
     */
    static void updateData(uint8_t *data){
        for(uint8_t i = 0; i < 60; i++){
            data[i] = _data[i];
        }
    }

    /*
     * @brief Transmits an array of data via UART
     * 
     * @param data A pointer to the array of data being sent, must have sizeof(data) == 60
     * @param direction 0: data is being sent to PC 1: data is being sent to another Teensy towards PC 2: data is being sent to another Teensy away from PC
     */
    static const void transmitData(const uint8_t *data, uint8_t direction){ 
        if(direction == 0){
            Serial.write(data, 60);
        }
        else if(direction == 1){
            Serial1.write(data, 60);
        }
        else{
            Serial2.write(data, 60);
        }
    }
    
};

//Definition of static variables
uint16_t Comms::_is_enabled = 0;
uint16_t Comms::_baud_rate[9] = {0};
HardwareSerial *Comms::_serial_ports[9];
uint8_t Comms::_sensor_list[64][4];

uint8_t Comms::_address;
uint8_t Comms::_packet_type;
uint8_t Comms::_data[60];
uint8_t Comms::_data2[60];
uint8_t Comms::_index = 0;
uint8_t Comms::_index2 = 0;
bool Comms::_has_new_packet = false;
bool Comms::_has_requested_data = false;
constexpr uint8_t Comms::_pingArray[60];


#endif