#include <PressureSensor.h>
#include <SD.h>


struct Data{
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkb; //Brake pressure in psi
  
} data;

File dataFile;
const int chipSelect = BUILTIN_SDCARD;
bool hasSD = false; //Keep false, will auto-set to true if it detects an SD card present
bool writeSerialMonitor = true; //Set to true if you want to troubleshoot values using Serial Monitor

uint16_t sdInterval     = 50 * 1000; //us  (50  ms)
uint16_t serialInterval = 50 * 1000; //us  (50  ms)
uint32_t sdTime = micros(), serialTime = micros(), oldTime = micros();


PressureSensor BrakeFront(A12, 2000, 4600, 8200), BrakeBack(A13, 2000, 4600, 8200); //inputPin, maxPressure (PSI), r1 (Ohms), r2 (Ohms)


void setup() {
  Serial.begin(115200); //For serial monitor


  BrakeFront.begin();
  BrakeBack.begin();

  
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed");
  } else {
    Serial.println("SD card initialization successful");
    hasSD = true;
  }
  
  delay(1000);
}

void loop() {

  if (false){ //Set to true if you want to print out the time in microseconds that each loop iteration takes
    uint32_t low = (micros() - oldTime) % 0xFFFFFFFF;
    Serial.print(low);
    Serial.println("us");
    oldTime = micros();
  }
  

  /**
   * If the time since last writing is greater than the interval time,
   * and if writeSerialMonitor is set to true then write data
   * THIS IS SOLELY FOR TESTING! COMMENT OUT BEFORE DOWNLOADING TO CAR
   */
  if (abs(micros() - serialTime) > serialInterval && writeSerialMonitor){
    collectAllData();
    Serial.print("BrakeFront: "+ String(data.brkf)  + "\t");
//    Serial.print("BrakeBack: "+ String(data.brkb)  + "\t");

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
  data.brkf  = BrakeFront.getPressurePSI();
  data.brkb  = BrakeBack.getPressurePSI();
}
