#include <FreqMeasureMulti.h>
#include <SPI.h>
#include <SD.h>
//Includes all necessary libraries
FreqMeasureMulti axle;
//FreqMeasureMulti engine;

//setting up global variables
uint32_t totTime = 30*1e6; //seconds before data collection stops (optional)
File dataFile;
File root;
File thisFile;
String fileName;
int numFiles = 1;
elapsedMillis timestamp;
int count = 1;
int save_rate = 1000;

byte dataArr[26];
  uint16_t LDS_FL; //LDS voltage readings (0-4095)
  uint16_t LDS_FR;
  uint16_t LDS_BL;
  uint16_t LDS_BR;
  uint16_t f_axle = 0; //axle and engine rotation speeds (rpm)
//  uint16_t f_engine = 0;
  uint16_t acc1_x; //accelerometer readings (0-4095)
  uint16_t acc1_y; //0 = -16g, 4095 = +16g
  uint16_t acc1_z;
  uint16_t acc2_x;
  uint16_t acc2_y;
  uint16_t acc2_z;
  uint32_t Time; //the time (microseconds)


void setup() {
  analogReadResolution(12);
  axle.begin(6);
//  engine.begin(6);
  
  pinMode(13,OUTPUT);
  if (!SD.begin(BUILTIN_SDCARD)) {
    while (1); //if there is a card issue, flash LED once every second
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
  fileNameCreator(); //run file name function
  delay(500); //wait for full initialisation of everything
  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  totTime = 0;
}



void loop() {
digitalWrite(13,HIGH);  
  if (count == 0){
    dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  }
    count++;
    
    LDS_FL = analogRead(A0);
    LDS_FR = analogRead(A1);
    LDS_BL = analogRead(A2);
    LDS_BR = analogRead(A3);
    //acc1 is 'blue', acc2 is 'yellow'
    acc1_x = analogRead(A15);
    acc1_y = analogRead(A13);
    acc1_z = analogRead(A12);
    acc2_x = analogRead(A17);
    acc2_y = analogRead(A16);
    acc2_z = analogRead(A14);

    if(axle.available()){
      f_axle = (uint8_t)(axle.countToFrequency(axle.read())*3); //coverts frequency to RPM (for 20 tooth wheel)
    }
//    if(engine.available()){
//      f_engine = (uint8_t)(engine.countToFrequency(axle.read())*3);
//    }

    Time = micros();
    
    dataArr[0] = (byte)LDS_FL;
    dataArr[1] = (byte)(LDS_FL >>8);
    dataArr[2] = (byte)LDS_FR;
    dataArr[3] = (byte)(LDS_FR >>8);
    dataArr[4] = (byte)LDS_BL;
    dataArr[5] = (byte)(LDS_BL >>8);
    dataArr[6] = (byte)LDS_BR;
    dataArr[7] = (byte)(LDS_BR >>8);
    dataArr[8] = (byte)acc1_x;
    dataArr[9] = (byte)(acc1_x >>8);
    dataArr[10] = (byte)acc1_y;
    dataArr[11] = (byte)(acc1_y >>8);
    dataArr[12] = (byte)acc1_z;
    dataArr[13] = (byte)(acc1_z >>8);
    dataArr[14] = (byte)acc2_x;
    dataArr[15] = (byte)(acc2_x >>8);
    dataArr[16] = (byte)acc2_y;
    dataArr[17] = (byte)(acc2_y >>8);
    dataArr[18] = (byte)acc2_z;
    dataArr[19] = (byte)(acc2_z >>8);
    dataArr[20] = (byte)f_axle;
    dataArr[21] = (byte)(f_axle >>8);
    dataArr[22] = (byte)Time;
    dataArr[23] = (byte)(Time >>8);
    dataArr[24] = (byte)(Time >>16);
    dataArr[25] = (byte)(Time >>24);
    
    dataFile.write(dataArr, 26);
  if (count == save_rate){
    dataFile.close();
    count = 0;
  }
  
  delayMicroseconds(900); //when 0, collection rate ~2200S/s, when 1500 collection rate ~450S/s
  
  if(Time*1e3 > totTime){ //comment out this bit if data collection to be unlimited
    while(1){
      digitalWrite(13, LOW);
      delay(500);
      digitalWrite(13, HIGH);
      delay(500);
    }
  }
}

void fileNameCreator()
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
