#include "Differential_Controller.h"
#include <UARTComms.h>
#include <Sensor.h>


#define PIN_DIFF1 14
#define PIN_DIFF2 15
#define PIN_DIFF3 16
#define PIN_DIFF4 17
#define PIN_DIFF5 18
#define PIN_DIFF6 19

#define PIN_SWITCHLEFT 4
#define PIN_SWITCHRIGHT 5

#define PIN_HE1 2
#define PIN_HE2 3

#define PIN_MOTORPOS 20
#define PIN_MOTORNEG 21



Differential_Controller diff_controller(PIN_DIFF1, PIN_DIFF2, PIN_DIFF3, PIN_DIFF4, PIN_DIFF5, PIN_DIFF6, PIN_MOTORPOS, PIN_MOTORNEG, PIN_SWITCHLEFT, PIN_SWITCHRIGHT);
UARTComms dashboard_comms(115200, Serial2);


void setup() {
    dashboard_comms.begin();
    dashboard_comms.attach_output_sensor(diff_controller, GENERIC_DIFFPOSITION_READ);
    Serial.begin(9600);
    diff_controller.begin();
    pinMode(13, OUTPUT);
  
//  diff_controller.rotate_R();
//  delay(1000);  
//  diff_controller.rotate_F();
//  delay(500);

}

uint32_t temp = 0;
bool led_on = 0;

void loop() {
    dashboard_comms.update();
  
    uint32_t t = micros();
    if((t - temp) > 1000000) {
        led_on = !led_on;
        temp = t;
        digitalWrite(13, led_on);
    }
    diff_controller.update();
    delay(10);
}
