#include <SDWrite.h>
#include "MainDAQ.h"
#include <Sensor.h>


unsigned __exidx_start;
unsigned __exidx_end;

SDWrite sd(BUILTIN_SDCARD);

TimeSensor timeSensor (MICROS);
RTCSensor rtc;
LDS lds_fl(23, 225, false);
LDS lds_fr(22, 225, false);
LDS lds_bl(40, 200, true);
LDS lds_br(32, 200, true);
BrakePressureTransducer brakeSensor1 (THREEV3ANA_10);
BrakePressureTransducer brakeSensor2 (THREEV3ANA_9);
IMUSensor imu(CS_2);

void setup() {
  sd.begin("test1.bin");
  sd.attach_output_sensor(timeSensor, TIME_AUXDAQ_US);
  sd.attach_output_sensor(rtc, RTC_UNIXTIME);
  sd.attach_output_sensor(brakeSensor1, BRAKE_SENSOR1);
  sd.attach_output_sensor(brakeSensor2, BRAKE_SENSOR2);
  sd.attach_output_sensor(lds_bl, LDS_BACKLEFTSHOCK_MM);
  sd.attach_output_sensor(lds_br, LDS_BACKRIGHTSHOCK_MM);
  sd.attach_output_sensor(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  sd.attach_output_sensor(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  sd.attach_output_sensor(imu, IMU_SENSOR);
  rtc.begin();
  brakeSensor1.begin();
  brakeSensor2.begin();
  lds_bl.begin(); 
  lds_br.begin(); 
  lds_fr.begin();
  lds_fl.begin();
  imu.begin();

  Serial.begin(2000000);
}

uint32_t serial_time = 0;
void loop() {
  sd.update();
  uint32_t t = micros();
  if(t - serial_time >= 100000){
    Serial.print("Brake: ");
    Serial.print(brakeSensor1.get_data()); Serial.print(", ");
    Serial.println(brakeSensor2.get_data());
    Serial.print("LDS BL: ");
    Serial.println(lds_bl.get_data());
    Serial.print("LDS BR: ");
    Serial.println(lds_br.get_data());
    Serial.print("LDS FR: ");
    Serial.println(lds_fr.get_data());
    Serial.print("LDS FL: ");
    Serial.println(lds_fl.get_data());
    auto imu_data = imu.get_data();
    Serial.print("Accel: (");
    Serial.print(imu_data.acceleration.x); Serial.print(", ");
    Serial.print(imu_data.acceleration.y); Serial.print(", ");
    Serial.print(imu_data.acceleration.z);
    Serial.println(")");
    Serial.print("Gyro: (");
    Serial.print(imu_data.gyro.x); Serial.print(", ");
    Serial.print(imu_data.gyro.y); Serial.print(", ");
    Serial.print(imu_data.gyro.z);
    Serial.println(")");
    
    Serial.println();
    Serial.println();
    serial_time = t;
  }
}
//
///*************************************************************************/
//// “main.ino”
//
//#include <Sensor.h>
//#include <UARTComms.h>
//
//// Create UARTComms object for communication over Serial1 (RX1/TX1)
//UARTComms uart1(Serial1);
//// Create an LDS sensor
//LinearDisplacementSensor lds(1);
//// Create two HE speed sensors
//HallEffectSpeedSensor engine_rpm(6, 200);
//HallEffectSpeedSensor secondary_rpm;
//
//void setup(){
//	uart1.begin();
//	// Attach lds and engine_rpm as output sensors
//	uart1.attach_output_sensor(lds, LDS_TEST1);
//	uart1.attach_output_sensor(engine_rpm, ENGINE_RPM);
//	// secondary_rpm will be received over UART.
//	// Note: it can still be used in code exactly the same as engine_rpm
//	uart1.attach_input_sensor (secondary_rpm, SECONDARY_RPM);
//	// Setup the pins for the two recorded sensors
//	lds.begin();
//	engine_rpm.begin();
//}
//
//void loop() {
//	uart1.update();
//}
///*************************************************************************/
