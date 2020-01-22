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

const uint16_t target_enigine_speed = 2750; 
PoluluG2MotorDriver ecvt_driver(2.0,0.0,0.0,-255, 255);

LDS ecvt_lds(A0, 50); //inputPin, travelMM, isReversed = false #####NOTE: need to check the actual travel distance of this LDS
PressureSensor brake_front_sensor(A1, 2000), brake_back_sensor(A2, 2000); //inputPin, scale (PSI), offset = 0
HallEffectSpeedSensor engine_speed_sensor(6, 20), wheel_speed_sensor(5, 20); //inputPin, toneWheelTeeth, intervalLength = 50us, averagingAmount = 200

void setup() {
  Serial.begin(115200); //For serial monitor
  Serial2.begin(9600);  //For data transmission with DAQ Teensy

  ecvt_driver.begin(3,2);
  ecvt_lds.begin();
  brake_front_sensor.begin();
  brake_back_sensor.begin();
  engine_speed_sensor.begin();
  wheel_speed_sensor.begin();
 
  delay(200);
}

void loop() {
  if (false){ //Set to true if you want to print out the time in microseconds that each loop iteration takes
    Serial.print(oldTime);
    Serial.println("us");
    oldTime = micros();
  }
  
  engine_speed_sensor.updateSensor();
  wheel_speed_sensor.updateSensor();
  
  ecvt_driver.step_pid(512 - ecvt_lds.getRawAnalog());
  

  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    collectAllData();
    Serial.print("Time: "  + String(data.currTime)   + "\t");
    Serial.print("ecvt_lds: "  + String(data.lds)  + "\t");
    Serial.print("brake_front_sensor: "+ String(data.brkf)  + "\t");
    Serial.print("brake_back_sensor: "+ String(data.brkb)  + "\t");
    Serial.print("engine_speed_sensor: "+ String(data.espd)  + "\t");
    Serial.print("wheel_speed_sensor: " + String(data.wspd)  + "\t");
    Serial.println();

    serialTime = micros();
  }

}

inline void collectAllData(){
  data.lds  = ecvt_lds.getRawAnalog();
  data.espd  = engine_speed_sensor.getSpeed();
  data.wspd  = wheel_speed_sensor.getSpeed();
  data.brkf  = brake_front_sensor.getPressurePSI();
  data.brkb  = brake_back_sensor.getPressurePSI();
  data.currTime = micros();
}
