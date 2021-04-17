#include <UARTComms.h>
#include <Sensor.h>

#define LED 13

UARTComms uart(115200, Serial);
TimeSensor t(MICROS);
HallEffectSpeedSensor he_speed(5, 600);

uint32_t prev_time = micros();
bool led_state = 0;

void setup() {
  // put your setup code here, to run once:
  uart.begin();
  uart.attach_output_sensor(t, TIME_GENERIC);
  //uart.attach_output_sensor(he_speed, SPEED_GENERIC);

  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  uart.update();

  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(LED, led_state);
    prev_time = micros();
  }
}
