//HE sensor
#include <SpeedSensor.h>
#include <SerialComms.h>
#include <Block.h>
#include <BlockId.h>
#include <ADS8688.h>
#include <Potentiometer.h>

unsigned __exidx_start;
unsigned __exidx_end;

//SDWrite sd(BUILTIN_SDCARD);
SerialComms serial(Serial);

//Load Cell
#include <Wire.h>
#include <EEPROM.h> //Needed to record user settings

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class
Block<uint8_t> tare_scale;
Potentiometer<uint32_t> pot;
ADS8688 adc(35);

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
  pinMode(33, OUTPUT); digitalWrite(33, HIGH);
  pinMode(34, OUTPUT); digitalWrite(34, HIGH);

  adc.attach_sensor(pot, 0);

  Wire.begin();
  Wire.setClock(400000); //Qwiic Scale is capable of running at 400kHz if desired

  if (myScale.begin() == false)
  {
    while (1);
  }
  
  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM

  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel 
  myScale.setGain(NAU7802_GAIN_16);


  serial.begin(115200);
  serial.attach_output_block(myScale, FORCE_ENGINEDYNO_LBS);
  serial.attach_output_block(pot, TEST_SENSOR_0);
  serial.attach_input_block(tare_scale, COMMAND_TARE_LOAD_CELL);
}

void loop() {
  //sd.update();
  
  if (tare_scale.get_data()) //Tare the scale
    myScale.calculateZeroOffset();

  //Updating each output sensor
  myScale.update();
  pot.update();
  tare_scale.update();
  
  serial.update();
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