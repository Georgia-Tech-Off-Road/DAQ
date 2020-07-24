#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>

#define BNO055 0x28
  
//Adafruit_BNO055 bno = Adafruit_BNO055(55);
int data = 0;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  Wire.begin();
  Wire.beginTransmission(BNO055);
  Wire.endTransmission();
  
}

void loop(void) {
  for (int i = 0; i < 127; i++){
    Wire.beginTransmission(BNO055);
    Wire.write(i);
    Wire.endTransmission();
    Wire.requestFrom(BNO055, 8);
    if (Wire.available()){
      data = Wire.read();
      Serial.println(data);
    }
    delay(5);
  }
  
}
