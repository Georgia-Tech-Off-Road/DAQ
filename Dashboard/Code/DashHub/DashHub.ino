#include <Adafruit_TLC5947.h>
#include <ServoControl.h>
#include "DashDial.h"

Adafruit_TLC5947 l_driver(1, 3, 2, 4);
Adafruit_TLC5947 r_driver(1, 12, 11, 10);
Adafruit_TLC5947 h_driver(1, 13, 14, 15);

ServoControl l_servo(5, 270);
ServoControl r_servo(9, 270);

DashDial l_dash(l_driver, l_servo, 0, 63, 0, 270, 0, 4095);
DashDial r_dash(r_driver, r_servo, 0, 63, 0, 270, 0, 4095);

void setup(){
    Serial.begin(9600);
    l_dash.begin();
    r_dash.begin();
    h_driver.begin();
}

uint8_t prev_count = 0;

void loop(){
    // 0 - 63 count.
    // increments once every 2^19 us (0.524 seconds).
    
    uint8_t count = 0x3F & (micros() >> 19);
    if (count != prev_count){
        Serial.println(count);
        prev_count = count;
        l_dash.set(count);
        r_dash.set(count);
        l_dash.update();
        r_dash.update();
//        if (count < 24){
//            l_driver.writePWM(count, 255);
//            r_driver.writePWM(count, 255);
//            h_driver.writePWM(count, 255);
//            l_servo .writeDeg( (count * 270) / 24 );
//            r_servo .writeDeg( (count * 270) / 24 );
//        } else if (count < 48){
//            uint8_t led = count - 24;
//            l_driver.writePWM(led, 0);
//            r_driver.writePWM(led, 0);
//            h_driver.writePWM(led, 0);
//            l_servo .writeDeg( (led * 270) / 24 );
//            r_servo .writeDeg( (led * 270) / 24 );
//        } else if (count < 56){
//            uint16_t pwma = 32*(count - 48);
//            l_driver.writeAll(pwma);
//            r_driver.writeAll(pwma);
//            h_driver.writeAll(pwma);
//        } else {
//            uint16_t pwma = 32*(63 - count);
//            l_driver.writeAll(pwma);
//            r_driver.writeAll(pwma);
//            h_driver.writeAll(pwma);
//        }
        
    }

}
