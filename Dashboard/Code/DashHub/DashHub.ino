#include <Adafruit_TLC5947.h>
#include <ServoControl.h>
#include "DashDial.h"
#include "SevenSegment.h"
#include <UARTComms.h>
#include <Sensor.h>

Adafruit_TLC5947 l_driver(1, 3, 2, 4);
Adafruit_TLC5947 r_driver(1, 12, 11, 10);
Adafruit_TLC5947 h_driver(1, 13, 14, 15);

ServoControl l_servo(5, 270);
ServoControl r_servo(9, 270);

DashDial l_dash(l_driver, l_servo, 0, 255, 0, 200, 0, 4095);
DashDial r_dash(r_driver, r_servo, 0, 255, 0, 200, 0, 4095);

SevenSegment h_seg(h_driver);


void setup(){
    Serial.begin(9600);
    l_dash.begin();
    r_dash.begin();
    h_seg.begin();
    pinMode(33, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(29, OUTPUT);
}

uint8_t prev_count = 0;
uint8_t dialnum = 127;
void loop(){
    digitalWrite(33, 1);
    digitalWrite(31, 1);
    digitalWrite(29, 1);
    // 0 - 63 count.
    // increments once every 2^19 us (0.524 seconds).
    uint8_t count = 0xFF & (micros() >> 15);
    h_seg.set_dp(0, 1);
    if (count != prev_count){
        Serial.println(count);
        if(rand() % 2) dialnum += 5;
        else dialnum -= 5;
        prev_count = count;
        l_dash.set(count);
        r_dash.set(count);
        l_dash.update();
        r_dash.update();
        h_seg.set_digit(0, count % 10);
        h_seg.set_digit(1, (count / 10) % 10);
        h_seg.set_digit(2, (count / 100) % 10);
        h_seg.update();
    }
}
