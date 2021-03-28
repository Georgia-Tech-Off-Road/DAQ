#include <UARTComms.h>
#include <Sensor.h>
#include <vector>
#include <memory>

// NEEDED FOR STL LIBRARY ON TEENSY 3.X
#if not defined(__IMXRT1062__)
namespace std { 
  void __throw_bad_alloc()
  {
    Serial.println("Unable to allocate memory");
  }

  void __throw_length_error ( char const*e ) 
  {
    Serial.print("Length Error :");
    Serial.println(e);
  }
} 
#endif

UARTComms uart1 (115200, Serial1);
TimeSensor s1(MICROS);
TimeSensor r1(MICROS);
GenericSensor r2(TEST_SENSOR_2, 2);
LDS lds(A9, 100, false);
RTCSensor rtc;

void setup() {
  uart1.begin();
  uart1.attach_input_sensor(r1, TEST_SENSOR_1);
  uart1.attach_input_sensor(r2, TEST_SENSOR_2);
  uart1.attach_output_sensor(s1, TEST_SENSOR_0);
  uart1.attach_output_sensor(lds, LDS_GENERIC);
  uart1.attach_input_sensor(rtc, RTC_UNIXTIME);
  lds.begin();
  Serial.begin(2000000);
}

uint32_t wrote_t = 0;
void loop() {
  uint32_t t = micros();
  s1.unpack((byte*)&t);
  uart1.update();
  if((t - wrote_t) > 100000){
//    Serial.print("Curr Time: ");
//    Serial.println(t);
//    Serial.print("Rec1 Time: ");
//    Serial.println(r1.get_data());
//    Serial.print("Rec2 Time: ");
//    std::vector<byte> bruh2 = r2.get_data();
//    Serial.println(*((uint16_t*)bruh2.data()));
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
