#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


//Recieving end of wireless transmission

//###  EXPECTING BELOW FORMAT ###//
//    [0]  = frs;
//    [1]  = fls;
//    [2]  = brs;
//    [3]  = bls;
//    [4]  = steer;
//    [5]  = ecvt;
//    [6]  = throttle;
//    [7]  = hrs;
//    [8]  = mins;
//    [9]  = secs;
//    [10] = espd;
//    [11] = espd >> 8;
//    [12] = wspd;
//    [13] = wspd >> 8;
//    [14] = brkf;
//    [15] = brkf >> 8;
//    [16] = brkb;
//    [17] = brkb >> 8;
//    [18] = ((int16_t)(imu1.w()*(1 << 14)));
//    [19] = ((int16_t)(imu1.w()*(1 << 14))) >> 8;
//    [20] = ((int16_t)(imu1.x()*(1 << 14)));
//    [21] = ((int16_t)(imu1.x()*(1 << 14))) >> 8;
//    [22] = ((int16_t)(imu1.y()*(1 << 14)));
//    [23] = ((int16_t)(imu1.y()*(1 << 14))) >> 8;
//    [24] = ((int16_t)(imu1.z()*(1 << 14)));
//    [25] = ((int16_t)(imu1.z()*(1 << 14))) >> 8;
//    [26] = ((int16_t)(imu2.w()*(1 << 14)));
//    [27] = ((int16_t)(imu2.w()*(1 << 14))) >> 8;
//    [28] = ((int16_t)(imu2.x()*(1 << 14)));
//    [29] = ((int16_t)(imu2.x()*(1 << 14))) >> 8;
//    [30] = ((int16_t)(imu2.y()*(1 << 14)));
//    [31] = ((int16_t)(imu2.y()*(1 << 14))) >> 8;
//    [32] = ((int16_t)(imu2.z()*(1 << 14)));
//    [33] = ((int16_t)(imu2.z()*(1 << 14))) >> 8;
//    [34] = ((int16_t)(accel1.x()*100));
//    [35] = ((int16_t)(accel1.x()*100)) >> 8;
//    [36] = ((int16_t)(accel1.y()*100));
//    [37] = ((int16_t)(accel1.y()*100)) >> 8;
//    [38] = ((int16_t)(accel1.z()*100));
//    [39] = ((int16_t)(accel1.z()*100)) >> 8;
//    [40] = ((int16_t)(euler1.x()*100));
//    [41] = ((int16_t)(euler1.x()*100)) >> 8;
//    [42] = ((int16_t)(euler1.y()*100));
//    [43] = ((int16_t)(euler1.y()*100)) >> 8;
//    [44] = ((int16_t)(euler1.z()*100));
//    [45] = ((int16_t)(euler1.z()*100)) >> 8;
//    [46] = ((int16_t)(gyro1.x()*100));
//    [47] = ((int16_t)(gyro1.x()*100)) >> 8;
//    [48] = ((int16_t)(gyro1.y()*100));
//    [49] = ((int16_t)(gyro1.y()*100)) >> 8;
//    [50] = ((int16_t)(gyro1.z()*100));
//    [51] = ((int16_t)(gyro1.z()*100)) >> 8;
//    [52] = 0;
//    [53] = 0;
//    [54] = 0;
//    [55] = 0;
//    [56] = atime;
//    [57] = atime >> 8;
//    [58] = atime >> 16;
//    [59] = atime >> 24;
//    [60] = 0xF0;
//    [61] = 0x00;
//    [62] = 0x00;
//    [63] = 0x0F;
//### EXPECTING ABOVE FORMAT ###//

struct Data{
  uint8_t frs;   //Value needs to be bigger if travel is more than 255mm
  uint8_t fls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t brs;   //Value needs to be bigger if travel is more than 255mm
  uint8_t bls;   //Value needs to be bigger if travel is more than 255mm
  uint8_t steer; //Value needs to be bigger if travel is more than 255mm
  uint8_t ecvt;  //Value needs to be bigger if travel is more than 255mm
  uint8_t throttle; //Throttle position 0-55 degrees
  uint16_t espd; //Engine speed in rpm
  uint16_t wspd; //Wheel speed in rpm
  uint16_t brkf; //Brake pressure in psi
  uint16_t brkb; //Brake pressure in psi

  float imu1w;
  float imu1x;
  float imu1y;
  float imu1z;
  float imu2w;
  float imu2x;
  float imu2y;
  float imu2z;
  float accel1x;
  float accel1y;
  float accel1z;
  float accel2x;
  float accel2y;
  float accel2z;
  float gyro1x;
  float gyro1y;
  float gyro1z;
  float gyro2x;
  float gyro2y;
  float gyro2z;
  float euler1x;
  float euler1y;
  float euler1z;


  uint8_t hrs; //RTC hours (0-24)
  uint8_t mins; //RTC minutes (0-60)
  uint8_t secs; //RTC seconds (0-60)
  uint32_t atime; //Time in microseconds

  byte incomingBytes[64] = {0};

  inline void unpackData(){    
    frs  =   incomingBytes[0];
    fls  =   incomingBytes[1];
    brs  =   incomingBytes[2];
    bls  =   incomingBytes[3];
    steer=   incomingBytes[4];
    ecvt =   incomingBytes[5];
    throttle=incomingBytes[6];
    hrs  =   incomingBytes[7];
    mins =   incomingBytes[8];
    secs =   incomingBytes[9];
    espd =  (uint16_t)incomingBytes[10] | (uint16_t)incomingBytes[11] << 8;
    wspd =  (uint16_t)incomingBytes[12] | (uint16_t)incomingBytes[13] << 8;
    brkf =  (uint16_t)incomingBytes[14] | (uint16_t)incomingBytes[15] << 8;
    brkb =  (uint16_t)incomingBytes[16] | (uint16_t)incomingBytes[17] << 8;
    imu1w = (float)(int16_t)(((int16_t)incomingBytes[18] | (int16_t)incomingBytes[19] << 8)) / (1 << 14);
    imu1x = (float)(int16_t)(((int16_t)incomingBytes[20] | (int16_t)incomingBytes[21] << 8)) / (1 << 14);
    imu1y = (float)(int16_t)(((int16_t)incomingBytes[22] | (int16_t)incomingBytes[23] << 8)) / (1 << 14);
    imu1z = (float)(int16_t)(((int16_t)incomingBytes[24] | (int16_t)incomingBytes[25] << 8)) / (1 << 14);
    imu1w = (float)(int16_t)(((int16_t)incomingBytes[26] | (int16_t)incomingBytes[27] << 8)) / (1 << 14);
    imu1x = (float)(int16_t)(((int16_t)incomingBytes[28] | (int16_t)incomingBytes[29] << 8)) / (1 << 14);
    imu1y = (float)(int16_t)(((int16_t)incomingBytes[30] | (int16_t)incomingBytes[31] << 8)) / (1 << 14);
    imu1z = (float)(int16_t)(((int16_t)incomingBytes[32] | (int16_t)incomingBytes[33] << 8)) / (1 << 14);
    accel1x=(float)(int16_t)(((int16_t)incomingBytes[34] | (int16_t)incomingBytes[35] << 8)) / 100;
    accel1y=(float)(int16_t)(((int16_t)incomingBytes[36] | (int16_t)incomingBytes[37] << 8)) / 100;
    accel1z=(float)(int16_t)(((int16_t)incomingBytes[38] | (int16_t)incomingBytes[39] << 8)) / 100;
    euler1x=(float)(int16_t)(((int16_t)incomingBytes[40] | (int16_t)incomingBytes[41] << 8)) / 100;
    euler1y=(float)(int16_t)(((int16_t)incomingBytes[42] | (int16_t)incomingBytes[43] << 8)) / 100;
    euler1z=(float)(int16_t)(((int16_t)incomingBytes[44] | (int16_t)incomingBytes[45] << 8)) / 100;
    gyro1x= (float)(int16_t)(((int16_t)incomingBytes[46] | (int16_t)incomingBytes[47] << 8)) / 100;
    gyro1y= (float)(int16_t)(((int16_t)incomingBytes[48] | (int16_t)incomingBytes[49] << 8)) / 100;
    gyro1z= (float)(int16_t)(((int16_t)incomingBytes[50] | (int16_t)incomingBytes[51] << 8)) / 100;
    atime = (uint32_t)incomingBytes[59] << 24 | (uint32_t)incomingBytes[58] << 16 | (uint32_t)incomingBytes[57] << 8 | (uint32_t)incomingBytes[56];

  }


  
} data;



uint8_t i = 0;
uint32_t tempCode = 0;
const uint32_t endCode = 0xF000000F;
bool byteArrayFull = false;
bool isError = false;
bool isOverflow = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(500);
  Serial.print("Initialized");

}

void loop() {
  if(Serial1.available()){
    data.incomingBytes[i] = Serial1.read();
    if(i >= 3) {
      tempCode = (uint32_t)data.incomingBytes[i - 3] << 24 | (uint32_t)data.incomingBytes[i - 2] << 16 | (uint32_t)data.incomingBytes[i - 1] << 8 | (uint32_t)data.incomingBytes[i];
      if(tempCode == endCode and i == 63){
        byteArrayFull = true;
      }
      else if(tempCode == endCode){
        isError = true;
      }
      else if(i == 63){
        isOverflow = true;
      }
    }
        
    i++;
    if(isError){
      Serial.println("Early endCode Error");
      isError = false;
      i = 0;
    }
    if(isOverflow){
      Serial.println("Overflow Error");
      isOverflow = false;
      i = 0;
    }
    if(byteArrayFull){
      byteArrayFull = false;
      i = 0;
      data.unpackData();
      //Serial.print("Time: " + String(data.atime) + "\t");
//      Serial.print(data.accel1x);
//      Serial.print(',');
//      Serial.print(data.accel1y);
//      Serial.print(',');
//      Serial.print(data.accel1z);
      //Serial.print("LDS: " + String(data.ecvt) + "\t");
      Serial.print("Engine: " + String(data.espd) + "\t");
      Serial.print("Wheel: " + String(data.wspd) + "\t");
      if(data.wspd != 0){
          Serial.print("Ratio: " + String(min(5000,1000 * (float)data.espd / (float)data.wspd)) + "\t");
      }
      Serial.println();
    }
  }
}
