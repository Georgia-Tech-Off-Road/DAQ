#include <SDWrite.h>
#include "MainDAQ.h"
#include <Sensor.h>

unsigned __exidx_start;
unsigned __exidx_end;

SDWrite sd(BUILTIN_SDCARD);

TimeSensor timeSensor (MICROS);
RTCSensor rtc;
HallEffectSpeedSensor speedSensor (THREEV3ANA_1, 20);
LDS lds_bl(23, 225, false);
LDS lds_br(22, 225, false);
LDS lds_fr(20, 200, false);
LDS lds_fl(21, 200, true);
BrakePressureTransducer brakeSensor1 (THREEV3ANA_10);
BrakePressureTransducer brakeSensor2 (THREEV3ANA_9);

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(timeSensor, TIME_AUXDAQ_US);
  sd.attach_output_sensor(rtc, RTC_UNIXTIME);
  sd.attach_output_sensor(speedSensor, SPEED_ENGINE_RPM); 
  sd.attach_output_sensor(brakeSensor1, BRAKE_SENSOR1);
  sd.attach_output_sensor(brakeSensor2, BRAKE_SENSOR2);
  // sd.attach_output_sensor(lds_bl, LDS_BACKLEFTSHOCK_MM);
  // sd.attach_output_sensor(lds_br, LDS_BACKRIGHTSHOCK_MM);
  sd.attach_output_sensor(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  sd.attach_output_sensor(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  lds_bl.begin(); 
  lds_br.begin(); 
  lds_fr.begin();
  lds_fl.begin();
  speedSensor.setup();
  brakeSensor1.begin();
  brakeSensor2.begin();
  rtc.begin();

  Serial.begin(2000000);
}

uint32_t serial_time = 0;
void loop() {
  sd.update();
  uint32_t t = micros();
  if(t - serial_time >= 100000){
    Serial.print("LDS BL: ");
    Serial.println(lds_bl.get_data());
    Serial.print("LDS BR: ");
    Serial.println(lds_br.get_data());
    Serial.print("LDS FR: ");
    Serial.println(lds_fr.get_data());
    Serial.print("LDS FL: ");
    Serial.println(lds_fl.get_data());
    Serial.println();
    serial_time = t;
  }
}
