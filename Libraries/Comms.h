#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

/*
 * This utility class is designed for robust communication using UART protocols and is designed to work mainly with Teensy
 * and Arduino but should work with any device supporting UART communication.
 * 
 * To add support for a new sensor you must follow these steps:
 *  1. Add the name of the sensors to the sensor_name_t enum and assign a value of 1 higher than the previous sensor
 *  2. Add the number of bytes that will be sent for that sensor in the _sensor_bytes array
 *  3. If the number of sensor you added is larger than the current storage of sensor information (i.e. _sensor_is_sending, etc.),
 *     then increase the size of those arrays to match (i.e. if you added SENSOR_NAME = 64 then the size of the arrays needs
 *     to be 65)
 * 
 * To use this library in an .ino script you need to follow these steps:
 *  1. in void setup add:
 *      a. for every port you use you need to call Comms::enableSerial for that port
 *      b. call Comms::begin()
 *  2. in void loop add:
 *      a. call Comms::update() every loop (must be called at a high frequency) >> sending frequency
 *      b. write data to be sent using  Comms::writeData<SENSOR>(data)
 *      c. read data that has been sent to device using Comms::readData<SENSOR>(&data)
 *      d. optionally, call Comms::hasNewData(SENSOR) to check if data for that sensor has actually been recieved since last read.
 *         if data has not been recieved for that sensor, readData will return the last recieved value (or 0 if a value has never
 *         been received)
 * 
 * To determine the max baud rate, refer to the chart at the bottom of this website: https://www.pjrc.com/teensy/td_uart.html
 * 
 * @TODO:
 *  - Add support for Teensy < 4.0 and for Arduino
 *  - Make sure port settings default to neither for all ports
 *  - Add functionality for wririting data to SD card as well
 *  - Define all static variables
 *  - Do all the logic for the update function
 *  - Make the packData and unpackData functions
 */


enum sensor_name_t : uint8_t
{
    ENGINE_SPEED_RPM = 0,
    SECONDARY_SPEED_RPM = 1,
    FRONT_LEFT_SHOCK = 2,
    FRONT_RIGHT_SHOCK = 3,
    BACK_LEFT_SHOCK = 4,
    BACK_RIGHT_SHOCK = 5,
    TIME_MICROS = 6,
    ACCEL1_X = 7,
    ACCEL1_Y = 8,
    ACCEL1_Z = 9,
    GYRO1_ROLL = 10,
    GYRO1_YAW = 11,
    GYRO1_PITCH = 12,
    ACCEL2_X = 13,
    ACCEL2_Y = 14,
    ACCEL2_Z = 15,
    GYRO2_ROLL = 16,
    GYRO2_YAW = 17,
    GYRO2_PITCH = 18,
    IS_DATA_COLLECTING = 19,
    SD_CARD_FILENAME = 20
};

enum port_setting_t : uint8_t
{
    NEITHER = 0,
    SENDING = 1,
    RECEIVING = 2,
    BOTH = 3
};

class Comms {
private:
    Comms() {}

    // These arrays store the settings for each individual port
    static uint16_t _baud_rate[9];
    static HardwareSerial *_serial_ports[9];
    static port_setting_t _port_setting[9];
    static bool _is_settings_need_sent[9];
    static bool _has_updated_settings[9];
    static bool _has_settings_requested[9];
    static bool _is_connected[9];

    static constexpr uint8_t _sensor_bytes[] = {
        2, //0
        2, //1
        2, //2
        2, //3
        2, //4
        2, //5
        4, //6
        2, //7
        2, //8
        2, //9
        2, //10
        2, //11
        2, //12
        2, //13
        2, //14
        2, //15
        2, //16
        2, //17
        2, //18
        1, //19
        12 //20
    };

    // Make sure these 4 arrays have a large enough size
    // The first two arrays store port specific settings and the last two are data storage shared between all ports
    static bool _sensor_is_sending[9][64];
    static bool _sensor_is_receiving[9][64];
    static uint8_t _data_store[64][12];
    static bool _has_new_data[64]; // This means that the device has received new data for a specific sensor since last read

    // Currently accepts max 'packet' size of 72 bytes. If you try to send more it will break everything (It will compile/run, but no data will be received)
    // These arrays store temp data before unpacking/after packing
    static uint8_t _data_receive[9][72];
    static uint8_t _index[9];
    static uint8_t _data_send[72];

    // These variables are used to make sure data is sent periodically and to check connectivity
    static uint32_t _last_time;
    static uint32_t _last_verified_connection;
    static uint16_t _period;

    static constexpr uint8_t _end_code[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0};

    static void sendData(uint8_t num_elements){
        for (int i = 0; i <= 8; i++){
            if (_port_setting[i] == SENDING || _port_setting[i] == BOTH){
                if (_has_updated_settings[i]){
                    if(_has_settings_requested[i]){
                        // Send settings
                        _has_updated_settings[i] = false;
                        _has_settings_requested[i] = false;
                    }
                    // else do nothing
                }
                else {
                    // Send the data
                    _serial_ports[i]->write(_data_send, num_elements);
                }
            }
        }
    }

    // This function should actually get the number of bytes being sent instead of the number of sensors
    static uint8_t getNumActiveSensors(){
        uint8_t num_active_elements = 0;
        for (int i = 0; i < sizeof(_sensor_is_sending)/sizeof(_sensor_is_sending[0]); i++){
            num_active_elements += int(_sensor_is_sending[i]);
        }
        return num_active_elements;
    }


    // This funciton should iterate through _data_store, and if the corresponding sensor is sending, then take the amount of bytes that the corresponding sensor is, and put it into _data_send
    static uint8_t packData(){
        return 0;
    }


    // This function should iterate through _data_receive[serial_number] and if the corresponding sensor is being received on that port, then take the amount of bytes and put it into _data_store
    // Should also set _has_new_data for that sensor
    static void unpackData(uint8_t serial_number){

    }

    /*
     * @breif this function will check the _data_receive for a specific port to see if the end code has been received to signify a full packet has been received
     * 
     * @param serial_number This is the port to check
     * 
     * @return A boolean value to indicate if the end_code has been received
     */
    static bool receivedEndCode(uint8_t serial_number){
        uint8_t end_code_bytes = sizeof(_end_code)/sizeof(_end_code[0]);
        if (_index[serial_number] < (end_code_bytes - 1)){
            return false;
        }
        for (int i = 0; i < end_code_bytes; i++){
            if (_end_code[i] != _data_receive[_index[serial_number] - (end_code_bytes - 1) + i]){
                return false;
            }
        }
        return true;
    }


public:
    /*
     * @brief Call these functions before the begin function to enable to respective Serial ports
     * 
     * @param serial_number The serial port which you wish to make active (must be a number from 0-8)
     * @param baud_rate The baud rate to be used with the respective serial port
     * @param port_setting Whether or not that port is sending or receiving (options are SENDING, RECEIVING, or BOTH)
     */
    static void enableSerial(uint8_t serial_number, uint16_t baud_rate, port_setting_t port_setting) {
        _port_setting[serial_number] = port_setting;
        if(port_setting){
            _baud_rate[serial_number] = baud_rate;
        }  
    }


    /*
     * @brief Necessary to set up hardware used for communication
     * 
     * @param period The period in which data is sent in microseconds (us) (for 200Hz -> 5000us)
     *      **Note: Can't be greater than 65,535 us
     */
    static void begin(uint16_t period = 5000){
        _period = period;
        _serial_ports[0] = &Serial;
        _serial_ports[1] = &Serial1;
        _serial_ports[2] = &Serial2;
        _serial_ports[3] = &Serial3;
        _serial_ports[4] = &Serial4;
        _serial_ports[5] = &Serial5;
        _serial_ports[6] = &Serial6;
        _serial_ports[7] = &Serial7;
        #if defined(__IMXRT1052__)   // Only defined for Teensy 4.1
        _serial_ports[8] = &Serial8;
        #endif
        for(uint8_t i = 0; i <= 8; i++){
            if(_port_setting[i] == SENDING || _port_setting[i] == RECEIVING || _port_setting[i] == BOTH){
                _serial_ports[i]->begin(_baud_rate[i]);
            }
        }
        setup_sensor_list();
    }


    /*
     * @brief Run this every loop to recieve any bytes in the serial buffer and parse them
     *  
     */
    static void update(){
        bool send_data = abs(micros() - _last_time) > _period;
        if (send_data){
            _last_time = micros();
            packData();
            uint8_t num_elements = getNumActiveSensors();
            sendData(num_elements);
        }
        for (uint8_t i = 0; i <= 8; i++){
            // Could be recieving data, recieving an ack, or receiving a settings request
            if (_port_setting[i] != NEITHER && _serial_ports[i]->available()){
                _data_receive[_index[i]] = _serial_ports[i]->read();
                bool end_of_packet = receivedEndCode(i);
                if (end_of_packet){
                    _index[i] = 0;
                    // ########### Handle stuff here
                } else {
                    _index[i]++;
                }
            }

            // If the port is recieving data and it needs settings, send a settings request
            if ((_port_setting[i] == RECEIVING || _port_setting[i] == BOTH) && _is_settings_needed[i]){
                // Send a settings request
                _serial_ports[i]->write(0);
                // Write end code here too
            }
        }
    }
    

    /*
     * @brief Handler for data corresponding to a specific sensor
     * 
     * @param data The data for the corresponding sensor. Must be correct size for that sensor.
     * 
     * A call would look like Comms::writeData<ENGINE_SPEED_RPM>(data);
     */
    template<sensor_name_t sensor,
             class T,
             typename = typename std::enable_if<(sizeof(T) == _sensor_bytes[sensor])>::type >
    static void writeData(const T data) {
        memcpy(_data_store[sensor], &data, _sensor_bytes[sensor]);
    }


    /*
     * @brief Used to retrieve data corresponding to a specific sensor. Will return the last collected value or 0 if no values have been
     *        collected ever. Use hasNewData() to check if a value you are reading is new.
     * 
     * @param *data Pointer to the data for the corresponding sensor. Must be correct size for that sensor.
     * 
     * A call would look like Comms::readData<ENGINE_SPEED_RPM>(&data);
     */
    template<sensor_name_t sensor,
             class T,
             typename = typename std::enable_if<(sizeof(T) == _sensor_bytes[sensor])>::type >
    static void readData(T *data) {
        memcpy(data, _data_store[sensor], _sensor_bytes[sensor]);
        _has_new_data[sensor] = false;
    }


    /*
     * @brief Used to check if a sensor has recieved updated data since the last read.
     * 
     * @param sensor A sensor_name_t type that corresponds to which sensor you want to check.
     */
    static bool hasNewData(sensor_name_t sensor) {
        return _has_new_data[sensor];
    }

    /*
     * @brief Used to check if a port is currently connected (this auto-updates about every second)
     * 
     * @param serial_number The serial port which you wish to check the connection of (must be a number from 0-8)
     */
    static bool checkConnected(uint8_t serial_number){
        return _is_connected[serial_number];
    }

};


//Definition of static variables
uint16_t Comms::_is_enabled = 0;
uint16_t Comms::_baud_rate[9] = {0};
HardwareSerial *Comms::_serial_ports[9];
uint8_t Comms::_sensor_bytes[64];
bool Comms::_sensor_is_sending[64] = {0};
bool Comms::_sensor_is_receiving[64] = {0};
bool Comms::_is_port_sending[9];


#endif