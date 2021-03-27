#include <SDWrite.h>
#include "MainDAQ.h"
#include <Sensor.h>

unsigned __exidx_start;
unsigned __exidx_end;

SDWrite sd(BUILTIN_SDCARD);

TimeSensor timeSensor (MICROS);
HallEffectSpeedSensor speedSensor (THREEV3ANA_1, 20);
BrakePressureTransducer brakeSensor1 (THREEV3ANA_10);
BrakePressureTransducer brakeSensor2 (THREEV3ANA_9);

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(timeSensor, TIME_AUXDAQ_US);
  sd.attach_output_sensor(speedSensor, SPEED_ENGINE_RPM); 
  sd.attach_output_sensor(brakeSensor1, BRAKE_SENSOR1);
  sd.attach_output_sensor(brakeSensor2, BRAKE_SENSOR2);
}

void loop() {
    sd.update();
}
