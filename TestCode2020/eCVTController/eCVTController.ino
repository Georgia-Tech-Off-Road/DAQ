#include <LDS.h>
#include <PressureSensor.h>
#include <HallEffectSpeedSensor.h>
#include <PoluluG2MotorDriver.h>
#include <Arduino.h>


struct Data{
  uint16_t lds;  //(0-1023 Raw analog position)
  uint16_t espd; //Engine speed in rpm
  uint16_t wspd; //Wheel speed in rpm
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkb; //Brake pressure in psi
  uint16_t current; //Current (0-1023) V = I*20mV/A + 50mV

  uint32_t currTime; //Time in microseconds

  byte allData[4] = {0};

  void compressData(){
    allData[0] = lds;
    allData[1] = current >> 8;
    allData[2] = 0xFF;
    allData[3] = 0xFF;
  }
} data;

//#################################### DEFINITION OF ALL I/0 VARIABLES #####################################//
bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor
uint32_t serialInterval  = 50 * 1000, daqInterval = 50 * 1000;   //us (50  ms)
uint32_t serialTime = micros(), daqTime = micros(), oldTime = micros();


//############################## DEFINITION OF ALL PIN ASSIGNMENTS AND OBJECTS ##############################//
uint8_t currSense = A1;
LDS ecvt_lds(A0, 50); //inputPin, travelMM, isReversed = false #####NOTE: need to check the actual travel distance of this LDS
PID ecvt_speed_pid;
PressureSensor brake_front_sensor(A2, 2000, 8200, 4700), brake_back_sensor(A3, 2000, 8200, 4700); //inputPin, scale (PSI), offset = 0
HallEffectSpeedSensor engine_speed_sensor(5, 6), wheel_speed_sensor(6, 86); //inputPin, toneWheelTeeth, intervalLength = 50us, averagingAmount = 200
PoluluG2MotorDriver ecvt_driver;

//################################# DEFINITION OF ALL ECVT RELATED VARIABLES ################################//
const uint16_t target_enigine_speed = 2850;
const uint16_t min_lds_pos = 389;
const uint16_t max_lds_pos = 710;
const uint16_t disengaged_target = 710;
const uint16_t threshold_rpm = 1800;
int16_t desired_power = 0;
bool hasRunFunction = false;



void setup() {
  Serial.begin(115200); //For serial monitor
  Serial3.begin(9600);  //For data transmission with DAQ Teensy

  ecvt_driver.begin(3,-2);
  PID * ecvt_pid = ecvt_driver.get_pid();

  //PID of just the motor
  ecvt_pid->set_constants(2.0,0.001,0.0);
  ecvt_pid->set_integral_cap(100000);
  ecvt_pid->set_power_bounds(-255,255);
  ecvt_pid->set_threshold_bounds(-50,25);

  //PID of the entire system
  ecvt_speed_pid.set_constants(2, 0.0, 1);
  ecvt_speed_pid.set_integral_cap(0);
  ecvt_speed_pid.set_derivative_bounds(-10000,0);
  ecvt_speed_pid.set_power_bounds(-255,255);
  ecvt_speed_pid.set_threshold_bounds(-50,25);

  
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

  //Only set one true at a time
  tuneClosingPosition(false);
  tuneOpeningPosition(false);
  runController(false);


/**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - daqTime) > daqInterval){
    daqTime = micros();
    collectAllData();
    data.compressData();
    Serial3.write(data.allData, sizeof(data.allData));
    //Serial.println("Full send");
  }
  
  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    collectAllData();
//    Serial.print("Time: "  + String(data.currTime)   + "\t");
    Serial.print("LDS: "  + String(data.lds)  + "\t");
//    Serial.print("brake_front_sensor: "+ String(data.brkf)  + "\t");
//    Serial.print("brake_back_sensor: "+ String(data.brkb)  + "\t");
//    Serial.print("Engine Speed: "+ String(data.espd)  + "\t");
//    Serial.print("Wheel Speed: " + String(data.wspd)  + "\t");
    Serial.println();

    serialTime = micros();
  }

}

inline void collectAllData(){ 
  data.lds  = ecvt_lds.getRawAnalog();
  data.espd  = engine_speed_sensor.getSpeed();
  data.wspd  = wheel_speed_sensor.getSpeed();
//  data.brkf  = brake_front_sensor.getPressurePSI();
//  data.brkb  = brake_back_sensor.getPressurePSI();
  data.current = analogRead(currSense);
  data.currTime = micros();
}

inline void tuneClosingPosition(bool runFunction){
  uint16_t i = 0;
  uint32_t j = 0;
  if(runFunction  && !hasRunFunction){
    delay(5000);
    Serial.println("Testing");
    data.lds = ecvt_lds.getRawAnalog();
    ecvt_driver.set_power(127);
    delay(500);
    ecvt_driver.set_power(0);
    delay(500);
    ecvt_driver.set_power(-127);
    while(j < 10){
      data.lds = ecvt_lds.getRawAnalog();
      if(data.lds >= min_lds_pos){
        ecvt_driver.set_power(-127);
        delayMicroseconds(3);
        j = 0;
      }
      else{
        ecvt_driver.set_power(0);
        delayMicroseconds(3);
        j++;
      }
      if(i > 100){
        Serial.println(data.lds);
        i = 0;
      }
      i++;
    }
    ecvt_driver.set_power(0);
    hasRunFunction = true;
  }
}

inline void tuneOpeningPosition(bool runFunction){
  uint16_t i = 0;
  uint32_t j = 0;
  if(runFunction && !hasRunFunction){
    delay(5000);
    Serial.println("Testing");
    data.lds = ecvt_lds.getRawAnalog();
    ecvt_driver.set_power(-127);
    delay(500);
    ecvt_driver.set_power(0);
    delay(500);
    ecvt_driver.set_power(127);
    while(j < 1000){
      data.lds = ecvt_lds.getRawAnalog();
      if(data.lds <= max_lds_pos){
        ecvt_driver.set_power(127);
        delayMicroseconds(3);
        j = 0;
      }
      else{
        ecvt_driver.set_power(0);
        delayMicroseconds(3);
        j++;
      }
      if(i > 100){
        Serial.println(data.lds);
        i = 0;
      }
      i++;
    }
    ecvt_driver.set_power(0);
    hasRunFunction = true;
  }
}

inline void runController(bool runFunction){
  if(runFunction){
    desired_power = ecvt_speed_pid.step(target_enigine_speed - data.espd);
    collectAllData();
    if(data.espd < threshold_rpm){
      // ECVT IS DISENGAGED
      ecvt_driver.set_power(127);
      ecvt_driver.step_pid(disengaged_target - data.lds);
    } else if(data.lds >= min_lds_pos && (data.lds <= max_lds_pos || desired_power < 0)) {  
      // ECVT IS ENGAGED
      Serial.println("---------");
      ecvt_driver.set_power(desired_power);
    } else {
      ecvt_driver.set_power(0);
    }
  }
}
