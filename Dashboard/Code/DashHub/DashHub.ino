#include <Adafruit_TLC5947.h>
#include <ServoControl.h>
#include "DashDial.h"
#include "SevenSegment.h"
#include <UARTComms.h>
#include <Sensor.h>
#include <SpeedSensor.h>
#include <Differential_Controller.h>

unsigned __exidx_start;
unsigned __exidx_end;


// TIRE RADIUS IN INCHES
#define TIRE_RADIUS 22
#define GEARBOX_RATIO 1

// --- Only have one uncommented at a time --- //
//#define RUN
//#define CALIBRATE
//#define TEST
#define DIFF_READ

UARTComms diff_comms(115200, Serial2);
Differential_Controller diff_controller();

Adafruit_TLC5947 l_driver(1, 3, 2, 4);
Adafruit_TLC5947 r_driver(1, 12, 11, 10);
Adafruit_TLC5947 h_driver(1, 13, 14, 15);
ServoControl l_servo(5, 270);
ServoControl r_servo(19, 270);
DashDial l_dash(l_driver, l_servo, 0, 400, 0, 500, 10, 265);
DashDial r_dash(r_driver, r_servo, 0, 4000, 0, 500, 10, 265);

SevenSegment h_seg(h_driver);

SpeedSensor engine_rpm_1 (22, 20, 255);
SpeedSensor engine_rpm_2 (1, 6, 255);
SpeedSensor secondary_rpm(20, 23, 255);

void setup(){
    diff_comms.begin();
    diff_comms.attach_input_sensor(diff_controller, GENERIC_DIFFPOSITION_READ);
    Serial.begin(2000000);
    l_dash.begin();
    r_dash.begin();
    h_seg.begin();
    pinMode(33, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(29, OUTPUT);
}

void loop(){
    #ifdef DIFF_READ
    diff_comms.update();
    static uint32_t diff_update = 0;
    if(abs(micros() - diff_update) > 100000){
        h_seg.update();
        h_seg.set_number(diff_controller.get_data());
    }
    #endif
    #ifdef RUN
    static uint32_t prev_change = 0;
    static uint32_t prev_update = 0;
    static int16_t var = 0;
    uint32_t t = micros();
    if(abs(t - prev_change) >= 1000){
      float in_per_min = secondary_rpm.get_data().speed * 2 * 3.1415 * TIRE_RADIUS / GEARBOX_RATIO;
      float mph = (in_per_min * 60.0) / (12.0 * 5280.0);
      uint16_t mph_10 = (uint16_t) (mph * 10);
      
//      Serial.println(mph);
      r_dash.set(engine_rpm_2.get_data().speed);
      l_dash.set(mph_10);
      h_seg.set_number(mph_10);
      h_seg.set_dp(1,1);
      prev_change = t;
    }
    if(abs(t - prev_update) >= 10000){
      l_dash.update();
      r_dash.update();
      h_seg.update();
      prev_update = t;
        Serial.print("\nEngine Speed 1  (RPM): ");
        Serial.println(engine_rpm_1.get_data().speed);
        Serial.print("Engine Speed 2  (RPM): ");
        Serial.println(engine_rpm_2.get_data().speed);
        Serial.print("Secondary Speed (RPM): ");
        Serial.println(secondary_rpm.get_data().speed);
//engine_rpm_1.get_rpm();
//engine_rpm_2.get_rpm();
//secondary_rpm.get_rpm();
    }
    #endif
    #ifdef TEST
    static uint32_t prev_update = 0;
    static uint32_t delay_update = 0;
    static uint16_t delay_state = 0;
    if(abs(micros() - prev_update) >= 10000){
      l_dash.update();
      r_dash.update();
      h_seg.update();
      prev_update = micros();
    }
    if(abs(micros() - delay_update) >= 20000){
      if(delay_state > 400){
        delayMicroseconds(2000000);
        delay_state = 0;
      }
      else if (delay_state == 1){
        delayMicroseconds(2000000);
      }
      if (delay_state <= 400){
        l_dash.set(delay_state);
        r_dash.set(delay_state*10);
        h_seg.set_number(delay_state);
        delay_state++;
      }
      delay_update = micros();
    }
    #endif
    #ifdef CALIBRATE
    l_dash.calibrate_servo(3);
    r_dash.calibrate_servo(3);
    #endif
}
