#include <LDS.h>
#include <PressureSensor.h>
#include <HallEffectSpeedSensor.h>
#include <PoluluG2MotorDriver.h>
#include <Arduino.h>


struct Data{
  uint16_t lds;  //Value needs to be bigger if travel is more than 255mm
  uint16_t espd; //Engine speed in rpm
  uint16_t wspd; //Wheel speed in rpm
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkb; //Brake pressure in psi

  uint32_t currTime; //Time in microseconds
} data;


bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor
uint16_t serialInterval  = 50 * 1000;   //us (50  ms)
uint32_t serialTime = micros(), oldTime = micros();

PoluluG2MotorDriver ecvt_driver(0.0,0.0,0.0,-255, 255);
LDS MotorLDS(A0, 50); //inputPin, travelMM, isReversed = false #####NOTE: need to check the actual travel distance of this LDS
PressureSensor BrakeFront(A1, 2000), BrakeBack(A2, 2000); //inputPin, scale (PSI), offset = 0
HallEffectSpeedSensor EngineSpeed(6, 20), WheelSpeed(5, 20); //inputPin, toneWheelTeeth, intervalLength = 50us, averagingAmount = 200

void setup() {
  Serial.begin(115200); //For serial monitor
  Serial2.begin(9600);  //For data transmission with DAQ Teensy

  ecvt_driver.begin(2,3);
  MotorLDS.begin();
  BrakeFront.begin();
  BrakeBack.begin();
  EngineSpeed.begin();
  WheelSpeed.begin();
 
  delay(200);
}

void loop() {
  if (false){ //Set to true if you want to print out the time in microseconds that each loop iteration takes
    Serial.print(oldTime);
    Serial.println("us");
    oldTime = micros();
  }
  
  EngineSpeed.updateSensor();
  WheelSpeed.updateSensor();
  

  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    collectAllData();
    Serial.print("Time: "  + String(data.currTime)   + "\t");
    Serial.print("MotorLDS: "  + String(data.lds)  + "\t");
    Serial.print("BrakeFront: "+ String(data.brkf)  + "\t");
    Serial.print("BrakeBack: "+ String(data.brkb)  + "\t");
    Serial.print("EngineSpeed: "+ String(data.espd)  + "\t");
    Serial.print("WheelSpeed: " + String(data.wspd)  + "\t");
    Serial.println();

    serialTime = micros();
  }

}

inline void collectAllData(){
  data.lds  = MotorLDS.getRawAnalog();
  data.espd  = EngineSpeed.getSpeed();
  data.wspd  = WheelSpeed.getSpeed();
  data.brkf  = BrakeFront.getPressurePSI();
  data.brkb  = BrakeBack.getPressurePSI();
  data.currTime = micros();
}
