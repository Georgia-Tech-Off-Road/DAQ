#include <SDWrite.h>
#include "MainDAQ.h"

SDWrite sd(BUILTIN_SDCARD);

HallEffectSpeedSensor speedSensor (3V3ANA_1, 20);
BrakePressureTransducer brakeSensor1 (3V3ANA_10);
BrakePressureTransducer brakeSensor2 (3V3ANA_9);

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(speedSensor, SPEED_ENGINE_RPM) 
  sd.attach_output_sensor(brakeSensor1, BRAKE_SENSOR1);
  sd.attach_output_sensor(brakeSensor2, BRAKE_SENSOR2);
}

void loop {
    sd.update();
}