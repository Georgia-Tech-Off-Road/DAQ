#include "UARTComms.h"
#include "SerialComms.h"
#include "TimeSensor.h"

#define LED_PIN 13

SerialComms uart_pc(Serial);
UARTComms uart_xbee(Serial2);

uint32_t prev_time = micros();
bool led_state = 0;

TimeSensor ts;
Block<uint32_t> block;

void setup() {
  uart_pc.begin(115200);
  uart_xbee.begin(115200);
  
  uart_xbee.attach_input_block(block, 105);
  uart_xbee.attach_output_block(ts, 100);

  uart_xbee.attach_throughput_comms(uart_pc);

  uart_pc.attach_output_block(ts, 100);
  
  uart_pc.attach_throughput_comms(uart_xbee);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  ts.update();
  uart_pc.update();
  uart_xbee.update();

  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    prev_time = micros();
    //Serial.println(block.get_data());
  } 
}
