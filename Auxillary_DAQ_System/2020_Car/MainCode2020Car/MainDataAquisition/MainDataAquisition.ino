i#include <LDS.h>
#include <HallEffectSpeedSensor.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

struct Data{
  uint8_t frs;   //Value needs to be bigger if travel is more than 255mm
  uint8_t fls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t brs;   //Value needs to be bigger if travel is more than 255mm
  uint8_t bls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t steer; //Value needs to be bigger if travel is more than 255mm
  uint8_t ecvt;  //Value needs to be bigger if travel is more than 255mm
  uint16_t espd; //Engine speed in rpm
  uint16_t wspd; //Wheel speed in rpm
  float imu1r;   //Roll in degrees
  float imu1p;   //Pitch in degrees
  float imu1y;   //Yaw/Heading in degrees
  float imu2r;   //Roll in degrees
  float imu2p;   //Pitch in degrees
  float imu2y;   //Yaw/Heading in degrees
  float accel1x; //Accelerometer x in m/s^2
  float accel1y; //Accelerometer y in m/s^2
  float accel1z; //Accelerometer z in m/s^2
  float accel2x; //Accelerometer x in m/s^2
  float accel2y; //Accelerometer y in m/s^2
  float accel2z; //Accelerometer z in m/s^2

  uint32_t atime; //Time in microseconds
  uint64_t other;

  uint8_t allDataSD[64] = {0};
  uint8_t allDataXBee[64] = {0};

  inline void packDataSD(){
    uint8_t temp[sizeof(float)];
    
    allDataSD[0] = frs;
    allDataSD[1] = fls;
    allDataSD[2] = brs;
    allDataSD[3] = bls;
    allDataSD[4] = steer;
    allDataSD[5] = ecvt;
    allDataSD[6] = espd >> 8;
    allDataSD[7] = espd;
    allDataSD[8] = wspd >> 8;
    allDataSD[9] = wspd;

    memcpy(&temp, &imu1r, sizeof(imu1r));   
    allDataSD[10] = temp[0];
    allDataSD[11] = temp[1];
    allDataSD[12] = temp[2];
    allDataSD[13] = temp[3];
    memcpy(&temp, &imu1p, sizeof(imu1p));
    allDataSD[14] = temp[0];
    allDataSD[15] = temp[1];
    allDataSD[16] = temp[2];
    allDataSD[17] = temp[3];
    memcpy(&temp, &imu1y, sizeof(imu1y));
    allDataSD[18] = temp[0];
    allDataSD[19] = temp[1];
    allDataSD[20] = temp[2];
    allDataSD[21] = temp[3];
    memcpy(&temp, &accel1x, sizeof(accel1x));
    allDataSD[22] = temp[0];
    allDataSD[23] = temp[1];
    allDataSD[24] = temp[2];
    allDataSD[25] = temp[3];
    memcpy(&temp, &accel1y, sizeof(accel1y));
    allDataSD[26] = temp[0];
    allDataSD[27] = temp[1];
    allDataSD[28] = temp[2];
    allDataSD[29] = temp[3];
    memcpy(&temp, &accel1z, sizeof(accel1z));
    allDataSD[30] = temp[0];
    allDataSD[31] = temp[1];
    allDataSD[32] = temp[2];
    allDataSD[33] = temp[3];
    memcpy(&temp, &imu2r, sizeof(imu2r));
    allDataSD[34] = temp[0];
    allDataSD[35] = temp[1];
    allDataSD[36] = temp[2];
    allDataSD[37] = temp[3];
    memcpy(&temp, &imu2p, sizeof(imu2p));
    allDataSD[38] = temp[0];
    allDataSD[39] = temp[1];
    allDataSD[40] = temp[2];
    allDataSD[41] = temp[3];
    memcpy(&temp, &imu2y, sizeof(imu2y));
    allDataSD[42] = temp[0];
    allDataSD[43] = temp[1];
    allDataSD[44] = temp[2];
    allDataSD[45] = temp[3];
    memcpy(&temp, &accel2x, sizeof(accel2x));
    allDataSD[46] = temp[0];
    allDataSD[47] = temp[1];
    allDataSD[48] = temp[2];
    allDataSD[49] = temp[3];
    memcpy(&temp, &accel2y, sizeof(accel2y));
    allDataSD[50] = temp[0];
    allDataSD[51] = temp[1];
    allDataSD[52] = temp[2];
    allDataSD[53] = temp[3];
    memcpy(&temp, &accel2z, sizeof(accel2z));
    allDataSD[54] = temp[0];
    allDataSD[55] = temp[1];
    allDataSD[56] = temp[2];
    allDataSD[57] = temp[3];
    allDataSD[58] = atime >> 24;
    allDataSD[59] = atime >> 16;
    allDataSD[60] = atime >> 8;
    allDataSD[61] = atime;

    allDataSD[62] = '\n';
  }

  inline void packDataXBee(){
    allDataXBee[0] = frs;
    allDataXBee[1] = fls;
    allDataXBee[2] = brs;
    allDataXBee[3] = bls;
    allDataXBee[4] = steer;
    allDataXBee[5] = ecvt;
    allDataXBee[6] = espd >> 8;
    allDataXBee[7] = espd;
    allDataXBee[8] = wspd >> 8;
    allDataXBee[9] = wspd; 
    allDataXBee[10] = convertFloatMSB(imu1r); //First 9 bits is the number before decimal [0-360)
    allDataXBee[11] = convertFloatLSB(imu1r); //Last 7 bits are number after decimal [0-100)
    allDataXBee[12] = convertFloatMSB(imu1p);
    allDataXBee[13] = convertFloatLSB(imu1p);
    allDataXBee[14] = convertFloatMSB(imu1y);
    allDataXBee[15] = convertFloatLSB(imu1y);
    allDataXBee[16] = convertFloatMSB(accel1x); //First 9 bits is the number before decimal (dependent on max g's measurement)
    allDataXBee[17] = convertFloatLSB(accel1x); //Last 7 bits are number after decimal [0-100)
    allDataXBee[18] = convertFloatMSB(accel1y);
    allDataXBee[19] = convertFloatLSB(accel1y);
    allDataXBee[20] = convertFloatMSB(accel1z);
    allDataXBee[21] = convertFloatLSB(accel1z);   
    
    allDataXBee[56] = atime >> 24;
    allDataXBee[57] = atime >> 16;
    allDataXBee[58] = atime >> 8;
    allDataXBee[59] = atime;
    allDataXBee[60] = 0xF0;
    allDataXBee[61] = 0x00;
    allDataXBee[62] = 0x00;
    allDataXBee[63] = 0x00;
  }

  inline uint8_t convertFloatMSB(float aflt){
    return (((int) aflt << 7) | (int) ((aflt - (int) aflt) * 100)) >> 8;
  }
  inline uint8_t convertFloatLSB(float aflt){
    return (((int) aflt << 7) | (int) ((aflt - (int) aflt) * 100));
  }
  
} data;

File dataFile;
const int chipSelect = BUILTIN_SDCARD;
bool hasSD = false;

uint16_t xbeeInterval = 50 * 1000; //us  (50 ms) max polling speed is about 10ms * # of 9DOF IMU's used + appropriate tolerance
uint16_t sdInterval   = 50  * 1000; //us  (50  ms)
uint64_t xbeeTime = micros();
uint64_t sdTime   = micros();
uint64_t oldTime  = micros();

LDS FRShock(A1, 200); //inputPin, travelMM, isReversed = false
LDS FLShock(A2, 200);
LDS BRShock(A3, 200);
LDS BLShock(A4, 200);
LDS Steer(A5, 50);
LDS ECVT(A6, 50);
PressureSensor Brake1(A7, 2000);
PressureSensor Brake2(A8, 2000);
HallEffectSpeedSensor EngineSpeed(9, 20); //inputPin, toneWheelTeeth, intervalLength = 0, averagingAmount = 1
HallEffectSpeedSensor WheelSpeed(10, 20);
Adafruit_BNO055 bno1 = Adafruit_BNO055(28, BNO055_ADDRESS_A); //0x28 address (ADR pin is left with default pull down resistor)
Adafruit_BNO055 bno2 = Adafruit_BNO055(29, BNO055_ADDRESS_B); //0x29 address (ADR pin is pulled high to 3.3V)


void setup() {
  Serial.begin(115200); //For serial monitor
  Serial1.begin(9600);  //For XBee

  FRShock.begin();
  FLShock.begin();
  BRShock.begin();
  BLShock.begin();
  Steer.begin();
  ECVT.begin();
  Brake1.begin();
  Brake2.begin();
  EngineSpeed.begin();
  WheelSpeed.begin();

  if(!bno1.begin()) {
    Serial.println("BNO055 1 initialization failed");
  } else {
    Serial.println("BNO055 1 initialization successful");
  }
  
  if(!bno2.begin()) {
    Serial.println("BNO055 2 initialization failed");
  } else {
    Serial.println("BNO055 2 initialization successful");
  }
  
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed");
  } else {
    Serial.println("SD card initialization successful");
    hasSD = true;
  }
  
  delay(1000);
  bno1.setExtCrystalUse(true);
  bno2.setExtCrystalUse(true);
}

void loop() {

  uint32_t low = (micros() - oldTime) % 0xFFFFFFFF;
  Serial.print(low);
  Serial.println("us");
  EngineSpeed.updateSensor();
  WheelSpeed.updateSensor();
  oldTime = micros();
  /**
   * If the time since last sending is greater than the interval time,
   * and if serial port has enough room in tx buffer then send xbee data
   */
  if ((abs(micros() - xbeeTime) > xbeeInterval) && (Serial1.availableForWrite() > sizeof(data.allDataXBee))){
    Serial.println("#########DATA GRAB####################");
    collectAllData();
    data.packDataXBee();
    Serial1.write(data.allDataXBee, sizeof(data.allDataXBee));
    xbeeTime = micros();
  }

  /**
   * If the time since last writing is greater than the interval time,
   * and if there is an SD card available to write to the write data
   */
  if (abs(micros() - sdTime) > sdInterval && hasSD){
    dataFile = SD.open("test.txt", FILE_WRITE);
    if (dataFile) {
      collectAllData();
      data.packDataSD();
      dataFile.write(data.allDataSD, sizeof(data.allDataSD));
      dataFile.println();
    } else {
      Serial.println("Error opening test.txt");
    }
    dataFile.close();
    sdTime = micros();
  }  

  if (abs(micros() - sdTime) > sdInterval){
    collectAllData();
    data.packDataSD();
    Serial.println("########DATA GRAB#######");
    Serial.write(data.allDataSD, sizeof(data.allDataSD));
    Serial.println();
//    for (int i = 0; i < sizeof(data.allDataSD); i++){
//      for(byte mask = 0x80; mask; mask >>= 1){
//         if(mask  & data.allDataSD[i]){
//             Serial.print('1');
//         } else {
//             Serial.print('0');
//         }
//      }
//    }

    sdTime = micros();
  }
  
}

inline void collectAllData(){
  sensors_event_t event1;
  sensors_event_t event2;

  data.frs   = FRShock.getPositionMM();
  data.fls   = FLShock.getPositionMM();
  data.brs   = BRShock.getPositionMM();
  data.bls   = BLShock.getPositionMM();
  data.steer = Steer.getPositionMM();
  data.ecvt  = ECVT.getPositionMM();
  data.espd  = EngineSpeed.getSpeed();
  data.wspd  = WheelSpeed.getSpeed();
  
//  bno1.getEvent(&event1);
//  data.accel1x = event1.acceleration.x;
//  data.accel1y = event1.acceleration.y;
//  data.accel1z = event1.acceleration.z;
//  data.imu1r = event1.orientation.roll;
//  data.imu1p = event1.orientation.pitch;
//  data.imu1y = event1.orientation.heading;
//  
//  bno2.getEvent(&event2);
//  data.accel2x = event2.acceleration.x;
//  data.accel2y = event2.acceleration.y;
//  data.accel2z = event2.acceleration.z;
//  data.imu2r = event2.orientation.roll;
//  data.imu2p = event2.orientation.pitch;
//  data.imu2y = event2.orientation.heading;

  data.atime = micros();
}
