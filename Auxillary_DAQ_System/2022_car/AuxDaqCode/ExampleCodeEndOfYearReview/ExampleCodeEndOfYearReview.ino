// Communication Libraries
#include "SerialComms.h"
#include "BlockId.h"
#include "LDS.h"
#include "ClockTimer.h"
#include "ADS8688.h"

#define LDS_PIN 0
#define LDS_STROKE 200
#define ADC1_CS 33
#define ADC2_CS 34
#define ADC3_CS 35
#define LED_PIN 30


SerialComms serial(Serial);
ADS8688 ads(35);
LDS<float> lds(LDS_STROKE);
ClockTimerf led_timer(2);

bool led_on = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  // put your setup code here, to run once:
  pinMode(ADC1_CS, INPUT_PULLUP);
  pinMode(ADC2_CS, INPUT_PULLUP);

  ads.attach_sensor(lds, LDS_PIN); //22 is pin on teensy
  SPI.begin();
  ads.begin(); //35 is chip select for ADC

  serial.begin(230400);
  serial.attach_output_block(lds, 406);

}

void loop() {
  // put your main code here, to run repeatedly:
  lds.update();
  serial.update();

  if(led_timer.ready(micros())) {
    led_on = !led_on;
    digitalWrite(LED_PIN, led_on);
  }
}
