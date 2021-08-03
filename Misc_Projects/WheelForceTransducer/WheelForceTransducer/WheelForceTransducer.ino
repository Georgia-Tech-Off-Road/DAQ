#include <UARTComms.h>
#include <SDWrite.h>
#include <Sensor.h>

#define LED_PIN 13

ADCSensor adc1;
ADCSensor adc2;
ADCSensor adc3;
// UARTComms uart(115200, Serial1);
// UARTComms pc(115200, Serial);
SDWrite sd(BUILTIN_SDCARD);
TimeSensor t1(MICROS);

uint32_t prev_time = micros();
bool led_state = 0;

void setup() {

  if (!adc1.begin(0x4A)) {
    while (1);
  }
  if (!adc2.begin(0x4B)) {
    while (1);
  }
  if (!adc3.begin(0x49)) {
    while (1);
  }
  

  adc1.setGain(GAIN_TWOTHIRDS);
  adc2.setGain(GAIN_TWOTHIRDS);
  adc3.setGain(GAIN_TWOTHIRDS);

  adc1.setDataRate(RATE_ADS1115_475SPS);
  adc2.setDataRate(RATE_ADS1115_475SPS);
  adc3.setDataRate(RATE_ADS1115_475SPS);
  
//  uart.begin();
//  uart.attach_output_sensor(adc1, WHEEL_FORCE_TRANSDUCER_ANALOG_1);
//  uart.attach_output_sensor(adc2, WHEEL_FORCE_TRANSDUCER_ANALOG_2);
//  uart.attach_output_sensor(adc3, WHEEL_FORCE_TRANSDUCER_ANALOG_3);

  sd.begin("wft1.bin");
  sd.attach_output_sensor(t1, TIME_AUXDAQ_US);
  sd.attach_output_sensor(adc1, WHEEL_FORCE_TRANSDUCER_ANALOG_1);
  sd.attach_output_sensor(adc2, WHEEL_FORCE_TRANSDUCER_ANALOG_2);
  sd.attach_output_sensor(adc3, WHEEL_FORCE_TRANSDUCER_ANALOG_3);
  

//  pc.begin();
//  pc.attach_output_sensor(adc1, WHEEL_FORCE_TRANSDUCER_ANALOG_1);
//  pc.attach_output_sensor(adc2, WHEEL_FORCE_TRANSDUCER_ANALOG_2);
//  pc.attach_output_sensor(adc3, WHEEL_FORCE_TRANSDUCER_ANALOG_3);
  

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
//  uart.update();
//  pc.update();
  sd.update();

  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    prev_time = micros();
  }  
}
