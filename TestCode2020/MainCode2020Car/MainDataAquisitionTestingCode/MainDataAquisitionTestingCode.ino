#include <LDS.h>
#include <PressureSensor.h>
#include <HallEffectSpeedSensor.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC

struct Data{
  uint8_t fls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t bls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t steer; //Value needs to be bigger if travel is more than 255mm
  uint16_t ecvt;  //(0-1023 analog position)
  uint8_t throttle; //Throttle position 0-55 degrees
  uint16_t espd; //Engine speed in rpm
  uint16_t wspd; //Wheel speed in rpm
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkb; //Brake pressure in psi
  uint8_t current; //Current
  uint16_t voltage;

  imu::Quaternion imu1;
  imu::Quaternion imu2;
  imu::Vector<3> accel1;
  imu::Vector<3> accel2;
  imu::Vector<3> gyro1;
  imu::Vector<3> gyro2;
  imu::Vector<3> euler1;

  uint8_t hrs; //RTC hours (0-24)
  uint8_t mins; //RTC minutes (0-60)
  uint8_t secs; //RTC seconds (0-60)
  uint32_t atime; //Time in microseconds

  uint8_t allDataSD[64] = {0};
  uint8_t allDataXBee[64] = {0};
  uint8_t incomingECVTData[13] = {0};

  inline void packDataSD(){    
    allDataSD[0]  = fls;
    allDataSD[1]  = bls;
    allDataSD[2]  = current;
    allDataSD[3]  = voltage;
    allDataSD[4]  = throttle;
    allDataSD[5]  = hrs;
    allDataSD[6]  = mins;
    allDataSD[7]  = secs;
    allDataSD[8]  = ecvt;
    allDataSD[9]  = ecvt >> 8;
    allDataSD[10] = espd;
    allDataSD[11] = espd >> 8;
    allDataSD[12] = wspd;
    allDataSD[13] = wspd >> 8;
    allDataSD[14] = brkf;
    allDataSD[15] = brkf >> 8;
    allDataSD[16] = brkb;
    allDataSD[17] = brkb >> 8;
    allDataSD[18] = ((int16_t)(imu1.w()*(1 << 14)));
    allDataSD[19] = ((int16_t)(imu1.w()*(1 << 14))) >> 8;
    allDataSD[20] = ((int16_t)(imu1.x()*(1 << 14)));
    allDataSD[21] = ((int16_t)(imu1.x()*(1 << 14))) >> 8;
    allDataSD[22] = ((int16_t)(imu1.y()*(1 << 14)));
    allDataSD[23] = ((int16_t)(imu1.y()*(1 << 14))) >> 8;
    allDataSD[24] = ((int16_t)(imu1.z()*(1 << 14)));
    allDataSD[25] = ((int16_t)(imu1.z()*(1 << 14))) >> 8;
    allDataSD[26] = ((int16_t)(imu2.w()*(1 << 14)));
    allDataSD[27] = ((int16_t)(imu2.w()*(1 << 14))) >> 8;
    allDataSD[28] = ((int16_t)(imu2.x()*(1 << 14)));
    allDataSD[29] = ((int16_t)(imu2.x()*(1 << 14))) >> 8;
    allDataSD[30] = ((int16_t)(imu2.y()*(1 << 14)));
    allDataSD[31] = ((int16_t)(imu2.y()*(1 << 14))) >> 8;
    allDataSD[32] = ((int16_t)(imu2.z()*(1 << 14)));
    allDataSD[33] = ((int16_t)(imu2.z()*(1 << 14))) >> 8;
    allDataSD[34] = ((int16_t)(accel1.x()*100));
    allDataSD[35] = ((int16_t)(accel1.x()*100)) >> 8;
    allDataSD[36] = ((int16_t)(accel1.y()*100));
    allDataSD[37] = ((int16_t)(accel1.y()*100)) >> 8;
    allDataSD[38] = ((int16_t)(accel1.z()*100));
    allDataSD[39] = ((int16_t)(accel1.z()*100)) >> 8;
    allDataSD[40] = ((int16_t)(euler1.x()*100));
    allDataSD[41] = ((int16_t)(euler1.x()*100)) >> 8;
    allDataSD[42] = ((int16_t)(euler1.y()*100));
    allDataSD[43] = ((int16_t)(euler1.y()*100)) >> 8;
    allDataSD[44] = ((int16_t)(euler1.z()*100));
    allDataSD[45] = ((int16_t)(euler1.z()*100)) >> 8;
    allDataSD[46] = ((int16_t)(gyro1.x()*100));
    allDataSD[47] = ((int16_t)(gyro1.x()*100)) >> 8;
    allDataSD[48] = ((int16_t)(gyro1.y()*100));
    allDataSD[49] = ((int16_t)(gyro1.y()*100)) >> 8;
    allDataSD[50] = ((int16_t)(gyro1.z()*100));
    allDataSD[51] = ((int16_t)(gyro1.z()*100)) >> 8;
    allDataSD[52] = 0;
    allDataSD[53] = 0;
    allDataSD[54] = 0;
    allDataSD[55] = 0;
    allDataSD[56] = atime;
    allDataSD[57] = atime >> 8;
    allDataSD[58] = atime >> 16;
    allDataSD[59] = atime >> 24;


  }

  inline void packDataXBee(){
    packDataSD();
    for (int i = 0; i < 60; i++){
      allDataXBee[i] = allDataSD[i];
    }
    allDataXBee[60] = 0xF0;
    allDataXBee[61] = 0x00;
    allDataXBee[62] = 0x00;
    allDataXBee[63] = 0x0F;
  }

  inline void unpackECVTData(){
    ecvt   = (uint16_t)incomingECVTData[0] | (uint16_t)incomingECVTData[1] << 8;
    espd   = (uint16_t)incomingECVTData[2] | (uint16_t)incomingECVTData[3] << 8;
    wspd   = (uint16_t)incomingECVTData[4] | (uint16_t)incomingECVTData[5] << 8;
    brkf   = (uint16_t)incomingECVTData[6] | (uint16_t)incomingECVTData[7] << 8;
    brkb   = (uint16_t)incomingECVTData[8] | (uint16_t)incomingECVTData[9] << 8;
    current= incomingECVTData[10];
  }
  
} data;


//##########  Definition of all data I/O objects  ############
File dataFile;
File root;
File thisFile;
String fileName;
int numFiles = 1;
const int chipSelect = BUILTIN_SDCARD;
bool hasSD = false; //Keep false, will auto-set to true if it detects an SD card present
bool useXbee = true; //Set to true if you want to use XBee
bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor
bool SDWorking = false, XbeeWorking = false, LEDState = 0;


//##########  Definition of all timer objects  ############
uint32_t xbeeInterval   = 100 * 1000; //us  (100 ms) max polling speed is about 10ms * # of 9DOF IMU's used + appropriate tolerance
uint32_t sdInterval     = 50 * 1000; //us  (50  ms)
uint32_t serialInterval = 50 * 1000; //us  (50  ms)
uint32_t xbeeTime = micros(), sdTime = micros(), serialTime = micros(), oldTime = micros(), LEDTime = micros(), currTime = micros();


//##########  Definition of all pin assignments and sensor objects  ############
LDS FLShock(A2, 225, true), BLShock(A6, 200), Steer(A3, 50), ECVT(A14, 50); //inputPin, travelMM, isReversed = false
PressureSensor BrakeFront(A12, 2000, 4600, 8200), BrakeBack(A13, 2000, 4600, 8200); //inputPin, maxPressure (PSI), r1 (Ohms), r2 (Ohms)
HallEffectSpeedSensor EngineSpeed(5, 6), WheelSpeed(6, 86); //inputPin, toneWheelTeeth, intervalLength = 50us, averagingAmount = 200
DS3232RTC RTC;
Adafruit_BNO055 bno1 = Adafruit_BNO055(28, BNO055_ADDRESS_A); //0x28 address (ADR pin is left with default pull down resistor)
Adafruit_BNO055 bno2 = Adafruit_BNO055(29, BNO055_ADDRESS_B); //0x29 address (ADR pin is pulled high to 3.3V)
bool hasbno1 = false, hasbno2 = false;
uint8_t throttlePin = A7;
uint8_t LEDPin = 13;
uint8_t batteryPin = A20;

//############ Variables for receiving data from the eCVT ####################
uint8_t indexer = 0;
uint16_t tempCode = 0;
const uint16_t endCode = 0xFFFF;
bool byteArrayFull = false, isError = false, isOverflow = false;


void setup() {
  Serial.begin(115200); //For serial monitor
  Serial1.begin(9600);  //For XBee
  Serial2.begin(9600);  //For getting data from eCVT

  FLShock.begin();
  BLShock.begin();
  Steer.begin();
  ECVT.begin();
  BrakeFront.begin();
  BrakeBack.begin();
  EngineSpeed.begin();
  WheelSpeed.begin();
  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  pinMode(throttlePin, INPUT);
  pinMode(LEDPin, OUTPUT);

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
    digitalWrite(LEDPin, LOW);
  } else {
    Serial.println("SD card initialization successful");
    hasSD = true;
    digitalWrite(LEDPin, HIGH);
  }
  fileNameCreator(); //run file name function
  delay(1000); //wait for full initialisation of everything
  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  bno1.setExtCrystalUse(true);
  bno2.setExtCrystalUse(true);
}

void loop() {

  if (false){ //Set to true if you want to print out the time in microseconds that each loop iteration takes
    oldTime = micros();
    uint32_t low = (micros() - oldTime) % 0xFFFFFFFF;
    Serial.print(low);
    Serial.println("us");
  }

  //These update the speed sensor values
  EngineSpeed.updateSensor();
  WheelSpeed.updateSensor();
  currTime = micros();
  
  /**
   * If the time since last sending is greater than the interval time,
   * and if serial port has enough room in tx buffer and if useXbee
   * is set to true then send xbee data
   */
  if ((abs(currTime - xbeeTime) > xbeeInterval) && (Serial1.availableForWrite() >= (sizeof(data.allDataXBee) - 1)) && useXbee){
    xbeeTime = micros();
    collectAllData();
    data.packDataXBee();
    Serial1.write(data.allDataXBee, sizeof(data.allDataXBee));
    XbeeWorking = true;
  }
  else if (abs(currTime - xbeeTime) > xbeeInterval){
    Serial.println(Serial1.availableForWrite());
    XbeeWorking = false;
  }

  /**
   * If the time since last writing is greater than the interval time,
   * and if there is an SD card available to write to the write data
   */
  if (abs(currTime - sdTime) > sdInterval && hasSD){
    sdTime = micros();
    dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (dataFile) {
      collectAllData();
      data.packDataSD();
      dataFile.write(data.allDataSD, sizeof(data.allDataSD));
      SDWorking = true;
    } else {
      SDWorking = false;
    }
    dataFile.close();
  }
  else if (abs(currTime - sdTime) > sdInterval){
    SDWorking = false;
  }

  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    serialTime = micros();
    collectAllData();
//    Serial.print("Time: "  + String(data.atime)   + "\t");
//    Serial.print("FLS: "   + String(data.fls)   + "\t");
//    Serial.print("BLS: "   + String(data.bls)   + "\t");
//    Serial.print("Steer: " + String(data.steer) + "\t");
//    Serial.print("ECVT: "  + String(data.ecvt)  + "\t");
//    Serial.print("Throttle: " + String(data.throttle) + " | ");
//    Serial.print("BrakeFront: "+ String(data.brkf)  + "\t");
//    Serial.print("BrakeBack: "+ String(data.brkb)  + "\t");
    Serial.print("Engine Speed: "+ String(data.espd)  + "\t");
    Serial.print("Wheel Speed: " + String(data.wspd)  + "\t");
//    Serial.print("X: " + String(data.imu1.x()) + " | ");
//    Serial.print("Y: " + String(data.imu1.y()) + " | ");
//    Serial.print("Z: " + String(data.imu1.z()) + " | ");
//    Serial.print("W: " + String(data.imu1.w()) + " | ");
//    Serial.print("X1: " + String(data.accel1.x()) + " | ");
//    Serial.print("Y1: " + String(data.accel1.y()) + " | ");
//    Serial.print("Z1: " + String(data.accel1.z()) + " | ");
    Serial.println();

  }

  /**
   * Controls the blinking of the LED on the Teensy
   * ON: The Xbee is transmitting and the SD card is writing
   * SLOW BLINK: The Xbee is transmitting
   * FAST BLINK: The SD Card is writing
   * OFF: Neither the Xbee is transmitting or SD card is writing (or the Teensy is not powered)
   */ 
  if (XbeeWorking && SDWorking){
    if (abs(micros() - LEDTime) > 200000){
      LEDTime = micros();
      LEDState = !LEDState;
      LEDState ? digitalWrite(LEDPin, HIGH) : digitalWrite(LEDPin, LOW);
    }
  }
  else if (XbeeWorking){
    if (abs(micros() - LEDTime) > 800000){
      LEDTime = micros();
      LEDState = !LEDState;
      LEDState ? digitalWrite(LEDPin, HIGH) : digitalWrite(LEDPin, LOW);
    }
  }
  else if (SDWorking){
    if (abs(micros() - LEDTime) > 1600000){
      LEDTime = micros();
      LEDState = !LEDState;
      LEDState ? digitalWrite(LEDPin, HIGH) : digitalWrite(LEDPin, LOW);
    }
  }
  else {
    digitalWrite(LEDPin, LOW);
    LEDState = 0;
  }

  /**
   * Recieves data from the DAQ platform
   */ 
 if(Serial2.available()){
    data.incomingECVTData[indexer] = Serial2.read();
    if(indexer >= 1) {
      tempCode = (uint16_t)data.incomingECVTData[indexer - 1] << 8 | (uint16_t)data.incomingECVTData[indexer];
      if(tempCode == endCode and indexer == sizeof(data.incomingECVTData) - 1){
        Serial.println("Got data!");
        byteArrayFull = true;
      }
      else if(tempCode == endCode){
        isError = true;
      }
      else if(indexer == sizeof(data.incomingECVTData) - 1){
        isOverflow = true;
      }
    }
        
    indexer++;
    if(isError){
      Serial.println("Early endCode Error");
      isError = false;
      indexer = 0;
    }
    if(isOverflow){
      Serial.println("Overflow Error");
      isOverflow = false;
      indexer = 0;
    }
    if(byteArrayFull){
      byteArrayFull = false;
      indexer = 0;
      data.unpackECVTData();
    }
  }
}

inline void fileNameCreator(){
  //Initializes SD card, checks how many existing files are on the card, numbers file accordingly
  root = SD.open("/");
  while (thisFile = root.openNextFile()) {
    if (!thisFile.isDirectory()) {
      numFiles++;
    }
  }
  fileName = (String) numFiles + ".txt";
  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  dataFile.close();
}

inline void collectAllData(){
  data.fls   = FLShock.getPositionMM();
  data.bls   = BLShock.getPositionMM();
  data.steer = Steer.getPositionMM();
  data.ecvt  = ECVT.getPositionMM();
  //data.espd  = EngineSpeed.getSpeed();
  //data.wspd  = WheelSpeed.getSpeed() * (86/25); //Scale by the number of teeth of measured gear vs. input shaft
  //data.brkf  = BrakeFront.getPressurePSI();
  //data.brkb  = BrakeBack.getPressurePSI();
  data.hrs   = hour();
  data.mins  = minute();
  data.secs  = second();
  
  data.voltage =  analogRead(batteryPin);
  data.throttle = map(analogRead(throttlePin),0,1023,55,0);

  if(hasbno1){
    data.imu1  = bno1.getQuat();
    data.accel1= bno1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL); //Make sure to change back to VECTOR_ACCELEROMETER
    data.gyro1 = bno1.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    data.euler1= bno1.getVector(Adafruit_BNO055::VECTOR_EULER); 
  }
  if(hasbno2){
    data.imu2  = bno1.getQuat();
    data.accel2= bno1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    data.gyro1 = bno1.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  }

  data.atime = micros();
}
