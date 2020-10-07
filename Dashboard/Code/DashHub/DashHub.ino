#include <TLC5947.h>
#include <ServoControl.h>

TLC5947 l_driver( 2,  3,  4, 30);
TLC5947 r_driver(12, 11, 10, 30);
TLC5947 h_driver(14, 13, 15, 30);

ServoControl l_servo(5, 270);
ServoControl r_servo(9, 270);

void setup(){
    l_driver.begin();
    r_driver.begin();
    h_driver.begin();
    l_servo .begin();
    r_servo .begin();
}

uint8_t prev_count = 0;

void loop(){
    // 0 - 63 count.
    // increments once every 2^19 us (0.524 seconds).
    uint8_t count = 0x3F & (micros() >> 19);

    if (count != prev_count){
        if (count < 24){
            l_driver.writePWM(count, 255);
            r_driver.writePWM(count, 255);
            h_driver.writePWM(count, 255);
            l_servo.writeDeg( (count * 270) / 24 );
            r_servo.writeDeg( (count * 270) / 24 );
        } else if (count < 48){
            uint8_t led = count - 24;
            l_driver.writePWM(led, 0);
            r_driver.writePWM(led, 0);
            h_driver.writePWM(led, 0);
            l_servo.writeDeg( (led * 270) / 24 );
            r_servo.writeDeg( (led * 270) / 24 );
        } else if (count < 56){
            uint16_t pwm = 32*(count - 48);
            l_driver.writeAll(pwm);
            r_driver.writeAll(pwm);
            h_driver.writeAll(pwm);
        } else {
            uint16_t pwm = 32*(63 - count);
            l_driver.writeAll(pwm);
            r_driver.writeAll(pwm);
            h_driver.writeAll(pwm);
        }
    }

    update();
}

inline void update(){
    l_driver.update();
    r_driver.update();
    h_driver.update();
}
