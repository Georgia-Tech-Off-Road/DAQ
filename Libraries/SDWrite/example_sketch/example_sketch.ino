#include <SDWrite.h>
#include <Sensor.h>

SDWrite sd(BUILTIN_SDCARD);
TimeSensor ts (MICROS);
HallEffectSpeedSensor engine_speed (1, 10);
LDS lds(A9, 100, false);

void setup() {
  sd.begin("test4.bin");
  sd.attach_output_sensor(ts, TIME_GENERIC);
  sd.attach_output_sensor(engine_speed, SPEED_GENERIC);
  sd.attach_output_sensor(lds, LDS_GENERIC);
  lds.begin();
  engine_speed.setup();
  Serial.begin(2000000);
}

uint32_t wrote_t = 0;
void loop() {
  uint32_t t = micros();
  sd.update();
}
