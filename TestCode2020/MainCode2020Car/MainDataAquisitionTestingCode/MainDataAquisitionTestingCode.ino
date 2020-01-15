#include <LDS.h>
#include <PressureSensor.h>
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
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkfr;//Brake pressure reference in psi
  uint16_t brkb; //Brake pressure in psi
  uint16_t brkbr;//Brake pressure reference in psi

  imu::Quaternion imu1;
  imu::Quaternion imu2;
  imu::Vector<3> accel1;
  imu::Vector<3> accel2;

  uint32_t atime; //Time in microseconds

  uint8_t allDataSD[64] = {0};
  uint8_t allDataXBee[64] = {0};

  inline void packDataSD(){    
    allDataSD[0] = frs;
    allDataSD[1] = fls;
    allDataSD[2] = brs;
    allDataSD[3] = bls;
    allDataSD[4] = steer;
    allDataSD[5] = ecvt;
    allDataSD[6] = (byte) espd >> 8;
    allDataSD[7] = (byte) espd;
    allDataSD[8] = (byte) wspd >> 8;
    allDataSD[9] = (byte) wspd;
    allDataSD[10] = (byte) brkf >> 8;
    allDataSD[11] = (byte) brkf;
    allDataSD[12] = (byte) brkfr >> 8;
    allDataSD[13] = (byte) brkfr;
    allDataSD[14] = (byte) brkb >> 8;
    allDataSD[15] = (byte) brkb;
    allDataSD[16] = (byte) brkbr >> 8;
    allDataSD[17] = (byte) brkbr;
    allDataSD[18] = (byte)((int16_t)imu1.w()*(1 << 14)) >> 8;
    allDataSD[19] = (byte)((int16_t)imu1.w()*(1 << 14));
    allDataSD[20] = (byte)((int16_t)imu1.x()*(1 << 14)) >> 8;
    allDataSD[21] = (byte)((int16_t)imu1.x()*(1 << 14));
    allDataSD[22] = (byte)((int16_t)imu1.y()*(1 << 14)) >> 8;
    allDataSD[23] = (byte)((int16_t)imu1.y()*(1 << 14));
    allDataSD[24] = (byte)((int16_t)imu1.z()*(1 << 14)) >> 8;
    allDataSD[25] = (byte)((int16_t)imu1.z()*(1 << 14));
    allDataSD[26] = (byte)((int16_t)imu2.w()*(1 << 14)) >> 8;
    allDataSD[27] = (byte)((int16_t)imu2.w()*(1 << 14));
    allDataSD[28] = (byte)((int16_t)imu2.x()*(1 << 14)) >> 8;
    allDataSD[29] = (byte)((int16_t)imu2.x()*(1 << 14));
    allDataSD[30] = (byte)((int16_t)imu2.y()*(1 << 14)) >> 8;
    allDataSD[31] = (byte)((int16_t)imu2.y()*(1 << 14));
    allDataSD[32] = (byte)((int16_t)imu2.z()*(1 << 14)) >> 8;
    allDataSD[33] = (byte)((int16_t)imu2.z()*(1 << 14));
    allDataSD[34] = (byte)((int16_t)accel1.x()*100) >> 8;
    allDataSD[35] = (byte)((int16_t)accel1.x()*100);
    allDataSD[36] = (byte)((int16_t)accel1.y()*100) >> 8;
    allDataSD[37] = (byte)((int16_t)accel1.y()*100);
    allDataSD[38] = (byte)((int16_t)accel1.z()*100) >> 8;
    allDataSD[39] = (byte)((int16_t)accel1.z()*100);
    allDataSD[40] = (byte)((int16_t)accel2.x()*100) >> 8;
    allDataSD[41] = (byte)((int16_t)accel2.x()*100);
    allDataSD[42] = (byte)((int16_t)accel2.y()*100) >> 8;
    allDataSD[43] = (byte)((int16_t)accel2.y()*100);
    allDataSD[44] = (byte)((int16_t)accel2.z()*100) >> 8;
    allDataSD[45] = (byte)((int16_t)accel2.z()*100);
    allDataSD[46] = 0;
    allDataSD[47] = 0;
    allDataSD[48] = 0;
    allDataSD[49] = 0;
    allDataSD[50] = 0;
    allDataSD[51] = 0;
    allDataSD[52] = (byte) atime >> 56;
    allDataSD[53] = (byte) atime >> 48;
    allDataSD[54] = (byte) atime >> 40;
    allDataSD[55] = (byte) atime >> 32;
    allDataSD[56] = (byte) atime >> 24;
    allDataSD[57] = (byte) atime >> 16;
    allDataSD[58] = (byte) atime >> 8;
    allDataSD[59] = (byte) atime;


  }

  inline void packDataXBee(){
    packDataSD();
    for (int i = 0; i < 60; i++){
      allDataXBee[i] = allDataSD[i];
    }
    allDataXBee[60] = 0xF0;
    allDataXBee[61] = 0x00;
    allDataXBee[62] = 0x00;
    allDataXBee[63] = 0x00;
  }
  
} data;

File dataFile;
const int chipSelect = BUILTIN_SDCARD;
bool hasSD = false; //Keep false, will auto-set to true if it detects an SD card present
bool useXbee = false; //Set to true if you want to use XBee
bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor

uint16_t xbeeInterval   = 50 * 1000; //us  (50 ms) max polling speed is about 10ms * # of 9DOF IMU's used + appropriate tolerance
uint16_t sdInterval     = 50 * 1000; //us  (50  ms)
uint16_t serialInterval = 50 * 1000; //us  (50  ms)
uint32_t xbeeTime = micros(), sdTime = micros(), serialTime = micros(), oldTime = micros();


LDS FRShock(A0, 200), FLShock(A1, 200), BRShock(A2, 200), BLShock(A3, 200), Steer(A4, 50), ECVT(A5, 50); //inputPin, travelMM, isReversed = false
PressureSensor BrakeFront(A6, 2000), BrakeFrontReference(A7, 2000), BrakeBack(A8, 2000), BrakeBackReference(A9, 2000); //inputPin, scale (PSI)
HallEffectSpeedSensor EngineSpeed(6, 20), WheelSpeed(10, 20); //inputPin, toneWheelTeeth, intervalLength = 50us, averagingAmount = 200
Adafruit_BNO055 bno1 = Adafruit_BNO055(28, BNO055_ADDRESS_A); //0x28 address (ADR pin is left with default pull down resistor)
Adafruit_BNO055 bno2 = Adafruit_BNO055(29, BNO055_ADDRESS_B); //0x29 address (ADR pin is pulled high to 3.3V)
bool hasbno1 = false, hasbno2 = false;



void setup() {
  Serial.begin(115200); //For serial monitor
  Serial1.begin(9600);  //For XBee

  FRShock.begin();
  FLShock.begin();
  BRShock.begin();
  BLShock.begin();
  Steer.begin();
  ECVT.begin();
  BrakeFront.begin();
  BrakeFrontReference.begin();
  BrakeBack.begin();
  BrakeBackReference.begin();
  EngineSpeed.begin();
  WheelSpeed.begin();

  if(!bno1.begin()) {
    Serial.println("BNO055 1 initialization failed");
  } else {
    Serial.println("BNO055 1 initialization successful");
    hasbno1 = true;
  }
  
  if(!bno2.begin()) {
    Serial.println("BNO055 2 initialization failed");
  } else {
    Serial.println("BNO055 2 initialization successful");
    hasbno2 = true;
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

  if (false){ //Set to true if you want to print out the time in microseconds that each loop iteration takes
    uint32_t low = (micros() - oldTime) % 0xFFFFFFFF;
    Serial.print(low);
    Serial.println("us");
    oldTime = micros();
  }
  
  EngineSpeed.updateSensor();
  WheelSpeed.updateSensor();
  
  /**
   * If the time since last sending is greater than the interval time,
   * and if serial port has enough room in tx buffer and if useXbee
   * is set to true then send xbee data
   */
  if ((abs(micros() - xbeeTime) > xbeeInterval) && (Serial1.availableForWrite() >= sizeof(data.allDataXBee)) && useXbee){
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
//    dataFile = SD.open("test.txt", FILE_WRITE);
//    if (dataFile) {
//      collectAllData();
//      data.packDataSD();
//      dataFile.write(data.allDataSD, sizeof(data.allDataSD));
//      dataFile.println();
//    } else {
//      Serial.println("Error opening test.txt");
//    }
//    dataFile.close();
//    sdTime = micros();
  }  

  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    collectAllData();
//    Serial.print("Time: "  + String(data.atime)   + "\t");
//    Serial.print("FRS: "   + String(data.frs)   + "\t");
//    Serial.print("FLS: "   + String(data.fls)   + "\t");
//    Serial.print("BRS: "   + String(data.brs)   + "\t");
//    Serial.print("BLS: "   + String(data.bls)   + "\t");
//    Serial.print("Steer: " + String(data.steer) + "\t");
//    Serial.print("ECVT: "  + String(data.ecvt)  + "\t");
//    Serial.print("BrakeFront: "+ String(data.brkf)  + "\t");
//    Serial.print("BrakeBack: "+ String(data.brkb)  + "\t");
    Serial.print("Engine Speed: "+ String(data.espd)  + "\t");
//    Serial.print("Wheel Speed: " + String(data.wspd)  + "\t");
//    Serial.print("Roll1: " + String(data.imu1r) + "\t");
//    Serial.print("Pitch1: " + String(data.imu1p) + "\t");
//    Serial.print("Yaw1: " + String(data.imu1y) + "\t");
//    Serial.print("X1: " + String(data.accel1x) + "\t");
//    Serial.print("Y1: " + String(data.accel1y) + "\t");
//    Serial.print("Z1: " + String(data.accel1z) + "\t");
//    Serial.print("Roll2: " + String(data.imu2r) + "\t");
//    Serial.print("Pitch2: " + String(data.imu2p) + "\t");
//    Serial.print("Yaw2: " + String(data.imu2y) + "\t");
//    Serial.print("X2: " + String(data.accel2x) + "\t");
//    Serial.print("Y2: " + String(data.accel2y) + "\t");
//    Serial.print("Z2: " + String(data.accel2z) + "\t");
    Serial.println();

    serialTime = micros();
  }

  /**
   * For writing to SD card
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - sdTime) > sdInterval && hasSD){
//    dataFile = SD.open("EDTEST5.txt", FILE_WRITE);
//    if (dataFile) {
//      collectAllData();
//      dataFile.print(data.atime);
//      dataFile.print(",");
//      dataFile.print(data.espd);
//      dataFile.println();
//    } else {
//      Serial.println("Error opening test.txt");
//    }
//    dataFile.close();
//    sdTime = micros();
  }  
}

inline void collectAllData(){
  data.frs   = FRShock.getPositionMM();
  data.fls   = FLShock.getPositionMM();
  data.brs   = BRShock.getPositionMM();
  data.bls   = BLShock.getPositionMM();
  data.steer = Steer.getPositionMM();
  data.ecvt  = ECVT.getPositionMM();
  data.espd  = EngineSpeed.getSpeed();
  data.wspd  = WheelSpeed.getSpeed();
  data.brkf  = BrakeFront.getPressurePSI();
  data.brkfr = BrakeFrontReference.getPressurePSI();
  data.brkb  = BrakeBack.getPressurePSI();
  data.brkbr = BrakeBackReference.getPressurePSI();

  if(hasbno1){
    data.imu1  = bno1.getQuat();
    data.accel1= bno1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  }
  if(hasbno2){
    data.imu2  = bno1.getQuat();
    data.accel2= bno1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  }

  data.atime = micros();
}
