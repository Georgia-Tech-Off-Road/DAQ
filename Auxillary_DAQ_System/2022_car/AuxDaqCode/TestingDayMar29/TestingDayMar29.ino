// Communication Libraries
#include "SerialComms.h"
#include "WirelessComms.h"
#include "SDComms.h"
#include "BlockId.h"

// Sensor Libraries
#include "TeensyADC.h"
#include "ADS8688.h"
#include "SpeedSensor.h"
#include "LDS.h"
#include "IMUSensor.h"
#include "TimeSensor.h"
#include "VoltageSensor.h"
#include "DigitalSensor.h"

// Control Libraries
#include "DigitalOutput.h"

// Utility
#include "ClockTimer.h"

// Pins and Constants
#define LDS_FL 0
#define LDS_FR 1
#define LDS_BL 2
#define LDS_BR 3
#define LDS_FL_STROKE 200
#define LDS_FR_STROKE 200
#define LDS_BL_STROKE 225
#define LDS_BR_STROKE 225

#define LED_ONBOARD       30
#define LED_PANEL_WHITE   9
#define LED_PANEL_RED     8
#define BTN_PANEL         7

WirelessComms wireless(Serial1);
SerialComms serial(Serial);

TeensyADC tadc;
ADS8688 ads(35);

LDS<float> lds_fl(LDS_FL_STROKE);
LDS<float> lds_fr(LDS_FR_STROKE);
LDS<float> lds_bl(LDS_BL_STROKE);
LDS<float> lds_br(LDS_BR_STROKE);
SpeedSensor speed_engine(10, 15);

DigitalSensor btn_panel;
DigitalOutput led_onboard;
DigitalOutput led_panel_white;
ClockTimer led_ct(1);
DigitalOutput led_panel_red;

Block<bool> sd_writecommand;

ClockTimerf serial_timer(200);

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  serial.begin(230400);
  wireless.begin(230400);
  
  ads.begin();
  ads.attach_sensor(lds_fl, LDS_FL);
  ads.attach_sensor(lds_fr, LDS_FR);
  ads.attach_sensor(lds_bl, LDS_BL);
  ads.attach_sensor(lds_br, LDS_BR);

  wireless.attach_input_block(sd_writecommand, COMMAND_AUXDAQ_SDWRITE);
  wireless.attach_output_block(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  wireless.attach_output_block(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  wireless.attach_output_block(lds_bl, LDS_BACKLEFTSHOCK_MM);
  wireless.attach_output_block(lds_br, LDS_BACKRIGHTSHOCK_MM);

  serial.attach_output_block(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  serial.attach_output_block(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  serial.attach_output_block(lds_bl, LDS_BACKLEFTSHOCK_MM);
  serial.attach_output_block(lds_br, LDS_BACKRIGHTSHOCK_MM);

  btn_panel.begin(BTN_PANEL, INPUT_PULLUP);
  led_onboard.begin(LED_ONBOARD, [](){ return sd_writecommand.get_data(); });
  led_panel_white.begin(LED_PANEL_WHITE, [](){ static bool state_ = 0; if(led_ct.ready(micros())){state_ = !state_;} return state_; });
  led_panel_red.begin(LED_PANEL_RED, [](){ return sd_writecommand.get_data(); });

  sd_writecommand.set_data(0);
  
  speed_engine.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t curr_time = micros();

  ads.update_sensors();
  speed_engine.update();

  btn_panel.update();
  led_onboard.update();
  led_panel_white.update();
  led_panel_red.update();

  serial.update();
  wireless.update();

  if(serial_timer.ready(curr_time) && 0){
//    Serial.print("TIME:   "); Serial.println(curr_time);
    Serial.print("FL LDS: "); Serial.println(lds_fl.get_data());
    Serial.print("FR LDS: "); Serial.println(lds_fr.get_data());
    Serial.print("BL LDS: "); Serial.println(lds_bl.get_data());
    Serial.print("BR LDS: "); Serial.println(lds_br.get_data());
//    Serial.print("ENGINE: "); Serial.println(speed_engine.get_data().speed);
//    Serial.print("VOLT:   "); Serial.println(vs.get_data());
//    Serial.print("DIGIT:  "); Serial.println(ds.get_data());
    Serial.print("BUTTON: "); Serial.println(btn_panel.get_data());
    Serial.print("WRITE:  "); Serial.println(sd_writecommand.get_data());
    Serial.println();
  }
}
