#include <UARTComms.h>
#include <Sensor.h>

// NEEDED FOR STL LIBRARY ON TEENSY 3.X

#if not defined(__IMXRT1062__)
namespace std {
void __throw_bad_alloc()
{
  Serial.println("Unable to allocate memory");
}

void __throw_length_error( char const*e )
{
  Serial.print("Length Error :");
  Serial.println(e);
}
}
#endif

UARTComms uart1 (115200, Serial1);
TimeSensor r1 (MICROS);
TimeSensor s1 (MICROS);
GenericSensor s2(TEST_SENSOR_2, 2);
LDS lds;
RTCSensor rtc;

void setup() {
  uart1.begin();
  uart1.attach_output_sensor(s1, TEST_SENSOR_1);
  uart1.attach_output_sensor(s2, TEST_SENSOR_2);
  uart1.attach_input_sensor (r1, TEST_SENSOR_0);
  uart1.attach_input_sensor (lds, LDS_GENERIC);
  uart1.attach_output_sensor(rtc, RTC_UNIXTIME);
  rtc.begin();
  Serial.begin(2000000);
}

uint32_t wrote_t = 0;
void loop() {
  uint32_t t = micros();
  uint16_t t2 = t << 1;
  s1.unpack((byte*)&t);
  s2.unpack((byte*)&t2);
  uart1.update();
  if((t - wrote_t) > 100000){
//    Serial.print("Curr Time: ");
//    Serial.println(t);
//    Serial.print("Reci Time: ");
//    Serial.println(r1.get_data());
//    Serial.print("Packet Size: ");
//    Serial.println(uart1.get_expected_receive_bytes());
//    Serial.print("LDS: ");
//    Serial.println(lds.get_data());
    DateTime now = rtc.get_data();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.println();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println("\n");
    wrote_t = t;
  }
}
