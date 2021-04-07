#include "Dyno.h"

//HE sensor
#include <SpeedSensor.h>
#include <SDWrite.h>
#include <Sensor.h>

unsigned __exidx_start;
unsigned __exidx_end;

SDWrite sd(BUILTIN_SDCARD);

//Load Cell
#include <Wire.h>
#include <EEPROM.h> //Needed to record user settings

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class
SpeedSensor engine_speed(4, H1, 255, CHANGE);
SpeedSensor secondary_speed(30, H2, 255, CHANGE);

//EEPROM locations to store 4-byte variables
#define LOCATION_CALIBRATION_FACTOR 0 //Float, requires 4 bytes of EEPROM
#define LOCATION_ZERO_OFFSET 10 //Must be more than 4 away from previous spot. Long, requires 4 bytes of EEPROM

bool settingsDetected = false; //Used to prompt user to calibrate their scale

//Create an array to take average of weights. This helps smooth out jitter.
#define AVG_SIZE 4
float avgWeights[AVG_SIZE];
byte avgWeightSpot = 0;



//Teensy SD card


void setup() {
  sd.begin("dyno1.bin");
  sd.attach_output_sensor(myScale, FORCE_ENGINEDYNO_LBS);
  sd.attach_output_sensor(engine_speed, SPEED_ENGINE_RPM);
  sd.attach_output_sensor(secondary_speed, SPEED_SECONDARY_RPM);
  
  
  pinMode(safeLED, OUTPUT);
  pinMode(testingLED, OUTPUT);
  pinMode(errorLED, OUTPUT);
  pinMode(shutdownSig, OUTPUT);
  pinMode(killSwitch, INPUT_PULLUP);

  //Load Cell
  Serial.begin(9600);

  Wire.begin();
  Wire.setClock(400000); //Qwiic Scale is capable of running at 400kHz if desired

  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Scale detected!");

  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM

  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel 
  myScale.setGain(NAU7802_GAIN_16);
  
  Serial.print("Zero offset: ");
  Serial.println(myScale.getZeroOffset());
  Serial.print("Calibration factor: ");
  Serial.println(myScale.getCalibrationFactor());
}

void loop() {
  sd.update();
  
  digitalWrite(testingLED, HIGH);


  //Load Cell
  if (myScale.available() == true)
  {
    float currentWeight = myScale.getWeight();



    avgWeights[avgWeightSpot++] = currentWeight;
    if(avgWeightSpot == AVG_SIZE) avgWeightSpot = 0;

    float avgWeight = 0;
    for (int x = 0 ; x < AVG_SIZE ; x++)
      avgWeight += avgWeights[x];
    avgWeight /= AVG_SIZE;

    Serial.print(avgWeight, 2); //Print 2 decimal places
    Serial.print("\tEngine Speed: \t");
    Serial.print(engine_speed.get_speed());
    Serial.print("\tSecondary Speed: \t");
    Serial.print(secondary_speed.get_speed());
    if(  digitalRead(killSwitch)) {
       Serial.print("\t Not Killed");
    } else {
       killed();
    }
    if(settingsDetected == false)
    {
      Serial.print("\tScale not calibrated. Press 'c'.");
    }

    Serial.println();
  }

  if (Serial.available())
  {
    byte incoming = Serial.read();

    if (incoming == 't') //Tare the scale
      myScale.calculateZeroOffset();
    else if (incoming == 'c') //Calibrate
    {
      calibrateScale();
    }
  }


  //Teensy SD card
  
}

void killed(void) {
  while (!digitalRead(killSwitch)) {
    digitalWrite(safeLED, LOW);
    digitalWrite(testingLED, LOW);
    digitalWrite(errorLED, HIGH);
    Serial.println("ESTOP IS PRESSED");
  }
    digitalWrite(testingLED, HIGH);
    digitalWrite(errorLED, LOW);
  return;
}
//Gives user the ability to set a known weight on the scale and calculate a calibration factor
void calibrateScale(void)
{
  Serial.println();
  Serial.println();
  Serial.println(F("Scale calibration"));

  Serial.println(F("Setup scale with no weight on it. Press a key when ready."));
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key

  myScale.calculateZeroOffset(64); //Zero or Tare the scale. Average over 64 readings.
  Serial.print(F("New zero offset: "));
  Serial.println(myScale.getZeroOffset());

  Serial.println(F("Place known weight on scale. Press a key when weight is in place and stable."));
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key

  Serial.print(F("Please enter the weight, without units, currently sitting on the scale (for example '4.25'): "));
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key

  //Read user input
  float weightOnScale = Serial.parseFloat();
  Serial.println();

  myScale.calculateCalibrationFactor(weightOnScale, 64); //Tell the library how much weight is currently on it
  Serial.print(F("New cal factor: "));
  Serial.println(myScale.getCalibrationFactor(), 2);

  Serial.print(F("New Scale Reading: "));
  Serial.println(myScale.getWeight(), 2);

  recordSystemSettings(); //Commit these values to EEPROM
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
