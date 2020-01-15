#include <iostream>
using namespace std;


struct Data{
  uint8_t frs  ;  //Value needs to be bigger if travel is more than 255mm
  uint8_t fls  ;  //Value needs to be bigger if travel is more than 255mm
  uint8_t brs  ;  //Value needs to be bigger if travel is more than 255mm
  uint8_t bls  ;  //Value needs to be bigger if travel is more than 255mm
  uint8_t steer;  //Value needs to be bigger if travel is more than 255mm
  uint8_t ecvt ;  //Value needs to be bigger if travel is more than 255mm
  uint16_t espd ; //Engine speed
  uint16_t wspd ; //Wheel speed
  uint16_t imu1r;  //Roll: Degrees before decimal point (constrained between 0-359)
  uint8_t imu1rd;  //Roll: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)
  uint16_t imu1p;  //Pitch: Degrees before decimal point (constrained between 0-359)
  uint8_t imu1pd;  //Pitch: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)
  uint16_t imu1y;  //Yaw: Degrees before decimal point (constrained between 0-359)
  uint8_t imu1yd;  //Yaw: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)
  uint16_t imu2r;  //Roll: Degrees before decimal point (constrained between 0-359)
  uint8_t imu2rd;  //Roll: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)
  uint16_t imu2p;  //Pitch: Degrees before decimal point (constrained between 0-359)
  uint8_t imu2pd;  //Pitch: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)
  uint16_t imu2y;  //Yaw: Degrees before decimal point (constrained between 0-359)
  uint8_t imu2yd;  //Yaw: Degrees after decimal point (to 2 decimal places ie constrained between 0-99)

  uint64_t allLDS;
  uint32_t allHES;
  uint64_t accel1;
  uint64_t accel2;
  uint64_t atime;
  uint64_t other;

  string alldata;

  // void packData(){
  //   allLDS = (frs << 56) | (fls << 48) | (brs << 40) | (bls << 32) | (steer << 24) | (ecvt << 16);
  //   allHES = (espd << 16) | (wspd);
  // }
  
} data;

uint16_t getDegrees(float aflt){
  return (int) aflt;
}
uint8_t getDegrees2(float aflt){
  return (aflt - (int) aflt) * 100;
}

int main(){
    float a = 100.1235;
    float b = 90.0000;
    float c = 359.636363;

    // cout << unsigned(getDegrees(a) << 7 | getDegrees2(a)) << endl; 
    // cout << unsigned(getDegrees(b) << 7 | getDegrees2(b)) << endl;
    // cout << unsigned(getDegrees(c) << 7 | getDegrees2(c)) << endl;
    uint8_t test[64] = {0};
    cout << sizeof(test);

    return 0;
}


