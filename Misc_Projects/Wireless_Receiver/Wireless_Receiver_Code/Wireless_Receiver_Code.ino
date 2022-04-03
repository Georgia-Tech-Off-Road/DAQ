#include <SerialComms.h>
#include <ClockTimer.h>

#define LED_PIN 13

SerialComms serial(Serial);
SerialComms xbee(Serial);
ClockTimerf serialTimer(100);
bool led_state = 0;

void setup() {
  serial.begin(115200);
  xbee.begin(115200);

  serial.attach_throughput_comms(xbee);
  xbee.attach_throughput_comms(serial);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  serial.update();
  xbee.update();

  if(serialTimer.ready(micros())){
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
  } 
}
