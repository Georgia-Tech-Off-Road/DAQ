#include "Neo6M.h"
#include "SerialComms.h"

SerialComms serial(Serial);

Neo6M gps(Serial2);

void setup() {    
  
  serial.begin(230400);
  gps.begin(57600);
  
}


void loop() {

  gps.update();
  static float mph10;

  mph10 = sqrt(gps.get_data().ecefv[0]*gps.get_data().ecefv[0] + 
                     gps.get_data().ecefv[1]*gps.get_data().ecefv[1] + 
                     gps.get_data().ecefv[2]*gps.get_data().ecefv[2]) * 10;

  Serial.println(mph10);
  serial.update();
  
}
