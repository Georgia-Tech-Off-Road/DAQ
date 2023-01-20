#ifndef IMUSENSOR_H
#define IMUSENSOR_H

#include <Adafruit_ISM330DHCX.h>
#include <Block.h>


class IMUSensor : public Block<imu_data_t> {
private:
    Adafruit_ISM330DHCX _ism;
    uint8_t _cs;
public:
    IMUSensor() : _cs(-1) { 
        _packlen = sizeof(float) * 7; 
    }
    
    void begin(uint8_t cs) { 
        _cs = cs;
        _ism.begin_SPI(_cs);
        _ism.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
        _ism.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
        _ism.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
        _ism.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);
        _ism.configInt1(false, false, true); // accelerometer DRDY on INT1 ?
        _ism.configInt2(false, true, false); // gyro DRDY on INT2 ?
    }

    void update () {
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        _ism.getEvent(&accel, &gyro, &temp);
        _data.acceleration.x = accel.acceleration.x;
        _data.acceleration.y = accel.acceleration.y;
        _data.acceleration.z = accel.acceleration.z;
        _data.gyro.x = gyro.gyro.x;
        _data.gyro.y = gyro.gyro.x;
        _data.gyro.z = gyro.gyro.z;
        _data.temperature = temp.temperature;
    }

    float get_accel_x() {
        return _data.acceleration.x;
    }

    float get_accel_y() {
        return _data.acceleration.y;
    }

    float get_accel_z() {
        return _data.acceleration.z;
    }

    float get_gyro_x() {
        return _data.gyro.x;
    }

    float get_gyro_y() {
        return _data.gyro.y;
    }

    float get_gyro_z() {
        return _data.gyro.z;
    }

    float get_temp() {
        return _data.temperature;
    }

    void printall() {
        Serial.print(get_accel_x()); Serial.print(", ");
        Serial.print(get_accel_y()); Serial.print(", ");
        Serial.print(get_accel_z()); Serial.print(", ");
        Serial.print(get_gyro_x()); Serial.print(", ");
        Serial.print(get_gyro_y()); Serial.print(", ");
        Serial.print(get_gyro_z()); Serial.print(", ");
        Serial.print(get_temp());
    }
    void printaccel() {
        Serial.print(get_accel_x()); Serial.print(", ");
        Serial.print(get_accel_y()); Serial.print(", ");
        Serial.print(get_accel_z());
    }

    void pack(byte* pack){
        float* p = (float*) pack;
        *(p++) = _data.acceleration.x;
        *(p++) = _data.acceleration.y;
        *(p++) = _data.acceleration.z;
        *(p++) = _data.gyro.x;
        *(p++) = _data.gyro.y;
        *(p++) = _data.gyro.z;
        *p     = _data.temperature;
    }
    void unpack(const byte* pack){
        const float* p = (const float*) pack;
        _data.acceleration.x = *(p++);
        _data.acceleration.y = *(p++);
        _data.acceleration.z = *(p++);
        _data.gyro.x         = *(p++);
        _data.gyro.y         = *(p++);
        _data.gyro.z         = *(p++)   ;
        _data.temperature    = *p;
    }
};

#endif