#include <UARTComms.h>
#include <Sensor.h>

UARTComms uart(2000000, Serial);
TimeSensor t(MICROS);
HallEffectSpeedSensor he_speed(5, 600);

void setup() {
  // put your setup code here, to run once:
  uart.begin();
  uart.attach_output_sensor(t, TIME_GENERIC);
  uart.attach_output_sensor(he_speed, SPEED_GENERIC);
}

void loop() {
  // put your main code here, to run repeatedly:
  uart.update();
}
