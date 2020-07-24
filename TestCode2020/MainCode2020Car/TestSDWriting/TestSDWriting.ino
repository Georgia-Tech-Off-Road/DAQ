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
  uint32_t atime; //Time in microseconds

  uint8_t allDataSD[8] = {0};

  inline void packDataSD(){    
//    allDataSD[0] = frs;
//    allDataSD[1] = fls;
//    allDataSD[2] = brs;
//    allDataSD[3] = bls;
//    allDataSD[4] = steer;
//    allDataSD[5] = ecvt;
//    allDataSD[6] = throttle;
//    allDataSD[7] = hrs;
//    allDataSD[8] = mins;
//    allDataSD[9] = secs;
//    allDataSD[10] = (byte) espd >> 8;
//    allDataSD[11] = (byte) espd;
//    allDataSD[12] = (byte) wspd >> 8;
//    allDataSD[13] = (byte) wspd;
//    allDataSD[14] = (byte) brkf >> 8;
//    allDataSD[15] = (byte) brkf;
//    allDataSD[16] = (byte) brkb >> 8;;
//    allDataSD[17] = (byte) brkb;
//    allDataSD[18] = (byte)(((int16_t)(imu1.w()*(1 << 14))) >> 8);
//    allDataSD[19] = (byte)((int16_t)(imu1.w()*(1 << 14)));
//    allDataSD[20] = (byte)((int16_t)(imu1.x()*(1 << 14))) >> 8;
//    allDataSD[21] = (byte)((int16_t)(imu1.x()*(1 << 14)));
//    allDataSD[22] = (byte)((int16_t)(imu1.y()*(1 << 14))) >> 8;
//    allDataSD[23] = (byte)((int16_t)(imu1.y()*(1 << 14)));
//    allDataSD[24] = (byte)((int16_t)(imu1.z()*(1 << 14))) >> 8;
//    allDataSD[25] = (byte)((int16_t)(imu1.z()*(1 << 14)));
//    allDataSD[34] = (byte)((int16_t)(accel1.x()*100)) >> 8;
//    allDataSD[35] = (byte)((int16_t)(accel1.x()*100));
//    allDataSD[36] = (byte)((int16_t)(accel1.y()*100)) >> 8;
//    allDataSD[37] = (byte)((int16_t)(accel1.y()*100));
//    allDataSD[38] = (byte)((int16_t)(accel1.z()*100)) >> 8;
//    allDataSD[39] = (byte)((int16_t)(accel1.z()*100));
//    allDataSD[46] = (byte)((int16_t)(gyro1.x()*100)) >> 8;
//    allDataSD[47] = (byte)((int16_t)(gyro1.x()*100));
//    allDataSD[48] = (byte)((int16_t)(gyro1.y()*100)) >> 8;
//    allDataSD[49] = (byte)((int16_t)(gyro1.y()*100));
//    allDataSD[50] = (byte)((int16_t)(gyro1.z()*100)) >> 8;
//    allDataSD[51] = (byte)((int16_t)(gyro1.z()*100));
//    allDataSD[52] = 0;
//    allDataSD[53] = 0;
//    allDataSD[54] = 0;
//    allDataSD[55] = 0;
    allDataSD[0] = (byte) (atime >> 24);
    allDataSD[1] = (byte) (atime >> 16);
    allDataSD[2] = (byte) (atime >> 8);
    allDataSD[3] = (byte) (atime);
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
bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor


//##########  Definition of all timer objects  ############
uint16_t sdInterval     = 100 * 1000; //us  (50  ms)
uint16_t serialInterval = 100 * 1000; //us  (50  ms)
uint32_t sdTime = micros(), serialTime = micros();


//##########  Definition of all pin assignments and sensor objects  ############
int LEDPin = 13;
uint32_t count = 0;


void setup() {
  Serial.begin(115200); //For serial monitor

  pinMode(LEDPin, OUTPUT);

  
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed");
  } else {
    Serial.println("SD card initialization successful");
    hasSD = true;
  }
  delay(1000);
  fileNameCreator(); //run file name function
  delay(1000); //wait for full initialisation of everything
  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
}

void loop() {
  /**
   * If the time since last writing is greater than the interval time,
   * and if there is an SD card available to write to the write data
   */
  if (abs(micros() - sdTime) > sdInterval && hasSD){
    dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (dataFile && count < 10) {
      digitalWrite(LEDPin, HIGH);
      data.atime = micros();
      data.packDataSD();
      dataFile.write(data.allDataSD, sizeof(data.allDataSD));
      Serial.print("File #: "   + String(fileName)   + "\t");
      Serial.print(data.allDataSD[0]);
      Serial.print('\t');
      Serial.print(data.allDataSD[1]);
      Serial.print('\t');
      Serial.print(data.allDataSD[2]);
      Serial.print('\t');
      Serial.print(data.allDataSD[3]);
      Serial.print('\t');
      Serial.print(data.allDataSD[4]);
      Serial.print('\t');
      Serial.print(data.allDataSD[5]);
      Serial.print('\t');
      Serial.print(data.allDataSD[6]);
      Serial.print('\t');
      Serial.print(data.allDataSD[7]);
      count++;
    } else {
      Serial.println("Error opening test.txt");
      digitalWrite(LEDPin, LOW);
    }
    dataFile.close();
    sdTime = micros();
//    Serial.print("Time: "  + String(data.atime)   + "\t");

    Serial.println();
    digitalWrite(LEDPin, LOW);
  }  
}



inline void fileNameCreator()
{
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
