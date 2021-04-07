#include <Adafruit_TLC5947.h>
#include <ServoControl.h>
#include "DashDial.h"
#include "SevenSegment.h"
#include <UARTComms.h>
#include <Sensor.h>

//#define CALIBRATE

Adafruit_TLC5947 l_driver(1, 3, 2, 4);
Adafruit_TLC5947 r_driver(1, 12, 11, 10);
Adafruit_TLC5947 h_driver(1, 13, 14, 15);
ServoControl l_servo(5, 270);
ServoControl r_servo(9, 270);
DashDial l_dash(l_driver, l_servo, 0, 4000, 0, 500, 10, 265);
DashDial r_dash(r_driver, r_servo, 0, 4000, 0, 500, 0, 265);

SevenSegment h_seg(h_driver);

HallEffectSpeedSensor engine_rpm_1 (22, 600);
HallEffectSpeedSensor engine_rpm_2 (6, 4);
HallEffectSpeedSensor secondary_rpm(23, 20);

void setup(){
    Serial.begin(2000000);
    l_dash.begin();
    r_dash.begin();
    h_seg.begin();
    engine_rpm_1.begin();
    engine_rpm_2.begin();
    secondary_rpm.begin();
    pinMode(33, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(29, OUTPUT);
}

void loop(){
    #ifndef CALIBRATE
    static uint32_t prev_change = 0;
    static uint32_t prev_update = 0;
    static int16_t var = 0;
    uint32_t t = micros();
    if((t - prev_change) >= 1000){
      l_dash.set(abs(var));
      r_dash.set(abs(var));
      h_seg.set_number(abs(var)/10);
      h_seg.set_dp(1,1);
      ++var;
      if(var > 4000) var = -4000;
      prev_change = t;
      Serial.println(var);
    }
    if((t - prev_update) >= 10000){
      l_dash.update();
      r_dash.update();
      h_seg.update();
      prev_update = t;
        Serial.print("\nEngine Speed 1  (RPM): ");
        Serial.println(engine_rpm_1.get_rpm());
        Serial.print("Engine Speed 2  (RPM): ");
        Serial.println(engine_rpm_2.get_rpm());
        Serial.print("Secondary Speed (RPM): ");
        Serial.println(secondary_rpm.get_rpm());
//engine_rpm_1.get_rpm();
//engine_rpm_2.get_rpm();
//secondary_rpm.get_rpm();
    }
    #else
    l_dash.calibrate_servo(3);
    r_dash.calibrate_servo(3);
    #endif
}
