#include <SDWrite.h>

SDWrite sd(BUILTIN_SDCARD);
GenericSensor s1(TEST_SENSOR_1, 4);
GenericSensor s2(TEST_SENSOR_2, 2);
HESpeedSensor engine_speed (23423, 2352);
GenericSensor chonk((sensor_id_t)1067, 60);

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(engine_speed, SPEED_ENGINE_RPM);
  Serial.begin(2000000);
}

uint32_t wrote_t = 0;
void loop() {
  // uint32_t t = micros();
  // uint16_t t2 = t << 1;
  // s1.unpack((byte*)&t);
  // s2.unpack((byte*)&t2);
  sd.update();
}
