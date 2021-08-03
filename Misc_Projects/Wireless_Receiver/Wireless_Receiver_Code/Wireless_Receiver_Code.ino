#include <UARTComms.h>

#define LED_PIN 13

UARTComms uart_pc(115200, Serial);
UARTComms uart_xbee(115200, Serial1);

uint32_t prev_time = micros();
bool led_state = 0;

void setup() {
  uart_pc.begin();
  uart_xbee.begin();

  uart_pc.attach_throughput_uart(uart_xbee);
  uart_xbee.attach_throughput_uart(uart_pc);

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
