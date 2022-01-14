#include "Dyno.h"

//HE sensor
//#include <SpeedSensor.h>
#include <Comms.h>
#include <Block.h>

unsigned __exidx_start;
unsigned __exidx_end;

//SDWrite sd(BUILTIN_SDCARD);
UARTComms uart(115200, Serial);

uint32_t prev_time = micros();
bool led_state = 0;

//Load Cell
#include <Wire.h>
#include <EEPROM.h> //Needed to record user settings

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class
SpeedSensor engine_speed(600, H1, 255);
SpeedSensor secondary_speed(30, H2, 255);
StateSensor tare_scale;

//EEPROM locations to store 4-byte variables
#define LOCATION_CALIBRATION_FACTOR 0 //Float, requires 4 bytes of EEPROM
#define LOCATION_ZERO_OFFSET 10 //Must be more than 4 away from previous spot. Long, requires 4 bytes of EEPROM

bool settingsDetected = false; //Used to prompt user to calibrate their scale

//Create an array to take average of weights. This helps smooth out jitter.
#define AVG_SIZE 4
float avgWeights[AVG_SIZE];
byte avgWeightSpot = 0;


void setup() {
  //sd.begin("dyno2.bin");
  //sd.attach_output_sensor(myScale, FORCE_ENGINEDYNO_LBS);
  //sd.attach_output_sensor(engine_speed, SPEED_ENGINE_RPM);
  //sd.attach_output_sensor(secondary_speed, SPEED_SECONDARY_RPM);
  pinMode(safeLED, OUTPUT);
  pinMode(testingLED, OUTPUT);
  pinMode(errorLED, OUTPUT);
  pinMode(shutdownSig, OUTPUT);
  pinMode(killSwitch, INPUT_PULLUP);


  Wire.begin();
  Wire.setClock(400000); //Qwiic Scale is capable of running at 400kHz if desired

  if (myScale.begin() == false)
  {
    digitalWrite(errorLED, HIGH);
    digitalWrite(testingLED, LOW);
    while (1);
  }
  
  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM

  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel 
  myScale.setGain(NAU7802_GAIN_16);


  uart.begin();
  uart.attach_output_sensor(myScale, FORCE_ENGINEDYNO_LBS);
  uart.attach_output_sensor(engine_speed, SPEED_DYNOENGINE600_RPM);
  uart.attach_output_sensor(secondary_speed, SPEED_DYNOSECONDARY30_RPM);
  uart.attach_input_sensor(tare_scale, COMMAND_TARE_LOAD_CELL);
  
}

void loop() {
  //sd.update();
  digitalWrite(testingLED, HIGH);
  
  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(safeLED, led_state);
    prev_time = micros();
  }


  if (tare_scale.get_state()) //Tare the scale
    myScale.calculateZeroOffset();


  uart.update();
}

void killed(void) {
  while (!digitalRead(killSwitch)) {
    digitalWrite(safeLED, LOW);
    digitalWrite(testingLED, LOW);
    digitalWrite(errorLED, HIGH);
  }
    digitalWrite(testingLED, HIGH);
    digitalWrite(errorLED, LOW);
  return;
}


//Record the current system settings to EEPROM
void recordSystemSettings(void)
{
  //Get various values from the library and commit them to NVM
  EEPROM.put(LOCATION_CALIBRATION_FACTOR, myScale.getCalibrationFactor());
  EEPROM.put(LOCATION_ZERO_OFFSET, myScale.getZeroOffset());
}

//Reads the current system settings from EEPROM
//If anything looks weird, reset setting to default value
void readSystemSettings(void)
{
  float settingCalibrationFactor; //Value used to convert the load cell reading to lbs or kg
  long settingZeroOffset; //Zero value that is found when scale is tared

  //Look up the calibration factor
  EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  if (settingCalibrationFactor == 0xFFFFFFFF)
  {
    settingCalibrationFactor = 0; //Default to 0
    EEPROM.put(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  }

  //Look up the zero tare point
  EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);
  if (settingZeroOffset == 0xFFFFFFFF)
  {
    settingZeroOffset = 1000L; //Default to 1000 so we don't get inf
    EEPROM.put(LOCATION_ZERO_OFFSET, settingZeroOffset);
  }

  //Pass these values to the library
  myScale.setCalibrationFactor(settingCalibrationFactor);
  myScale.setZeroOffset(settingZeroOffset);

  settingsDetected = true; //Assume for the moment that there are good cal values
  if (settingCalibrationFactor < 0.1 || settingZeroOffset == 1000)
    settingsDetected = false; //Defaults detected. Prompt user to cal scale.
}
