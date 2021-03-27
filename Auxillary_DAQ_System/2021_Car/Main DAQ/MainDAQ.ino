#include <SDWrite.h>
#include "MainDAQ.h"

SDWrite sd(BUILTIN_SDCARD);

HallEffectSpeedSensor speedSensor (3V3ANA_1, );
BrakePressureTransducer brakeSensor (, );

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(speedSensor, SPEED_ENGINE_RPM) 
  sd.attach_output_sensor(brakeSensor, BRAKE_SENSOR);
}

void loop {
    sd.update();
}