#include "UARTComms.h"
#include "SerialComms.h"

#define LED_PIN 13

SerialComms uart_pc(Serial);
UARTComms uart_xbee(Serial2);

uint32_t prev_time = micros();
bool led_state = 0;

void setup() {
  uart_pc.begin(115200);
  uart_xbee.begin(115200);

  uart_pc.attach_throughput_comms(uart_xbee);
  uart_xbee.attach_throughput_comms(uart_pc);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uart_pc.update();
  uart_xbee.update();

  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    prev_time = micros();
  } 
}
