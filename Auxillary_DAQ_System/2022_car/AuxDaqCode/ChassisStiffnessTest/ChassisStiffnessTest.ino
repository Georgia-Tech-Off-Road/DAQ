// Communication Libraries
#include "SerialComms.h"
#include "WirelessComms.h"
#include "BlockId.h"
#include "Block.h"

// Sensor Libraries
#include "LoadCell.h"
#include "ADS8688.h"

// Utility
#include "ClockTimer.h"

#define LOAD_CELL_PIN 0
#define ADC1_CS 33
#define ADC2_CS 34
#define ADC3_CS 35
#define LED_PIN 30


SerialComms serial(Serial);
WirelessComms wireless(Serial1);

ADS8688 ads(ADC3_CS); //35 is chip select for ADC
LoadCell load_cell;
Block<uint8_t> tare_scale;
ClockTimerf led_timer(2);

void setup() {
  pinMode(LED_PIN, OUTPUT);

  pinMode(ADC1_CS, INPUT_PULLUP);
  pinMode(ADC2_CS, INPUT_PULLUP);

  ads.attach_sensor(load_cell, LOAD_CELL_PIN);
  SPI.begin();
  ads.begin();

  load_cell.set_scale(1053.6); //increased from 881

  serial.begin(230400);
  wireless.begin(230400);
  serial.attach_output_block(load_cell, LOADCELL_CHASSISSTIFFNESS_LBS);
  serial.attach_input_block(tare_scale, COMMAND_TARE_LOAD_CELL);

  wireless.attach_output_block(load_cell, LOADCELL_CHASSISSTIFFNESS_LBS);
  wireless.attach_input_block(tare_scale, COMMAND_TARE_LOAD_CELL);
}

void loop() {
  load_cell.update();
  serial.update();
  wireless.update();

  if (tare_scale.get_data()) //Tare the scale
    load_cell.tare();

  if(led_timer.ready(micros())) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
