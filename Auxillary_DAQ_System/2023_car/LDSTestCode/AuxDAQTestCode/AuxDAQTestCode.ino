#include "AuxDAQTestCode.h"

#define DEBUG_PRINTING 1
#define USE_SERIAL 1

// Communications
WirelessComms wireless(Serial1);
SerialComms serial(Serial);
SDComms sdcomms;

// Sensors
TeensyADC tadc;
ADS8688 adc(ADC2);
LDS<float> lds_pedal1(LDS_PEDAL_STROKE);

TimeSensor ts;

// Control IO
DigitalSensor btn_panel;
DigitalOutput led_onboard;
DigitalOutput led_panel_white;
ClockTimerf led_panel_white_ct(1);
DigitalOutput led_panel_red;

EdgeDetector edge_detect;
Block<bool> wireless_writecommand;
DigitalOutput sd_writecommand;

ClockTimerf serial_timer(50);

void setup() {
  // Comms
  serial.begin(230400);
  wireless.begin(230400);
  sdcomms.set_sending_period(1000);

  // Sensors
  SPI.begin();
  adc.begin();
  pinMode(ADC1, OUTPUT); digitalWrite(ADC1, HIGH);
  pinMode(ADC2, OUTPUT); digitalWrite(ADC2, HIGH);
  adc.attach_sensor(lds_pedal1, 5);

  // Control IO
  btn_panel.begin(BTN_PANEL, INPUT_PULLUP);
   led_onboard.begin(LED_ONBOARD);
  led_onboard.set_setcb(MAKE_CB(sd_writecommand.get_data()));
  led_panel_white.begin(LED_PANEL_WHITE);
  led_panel_white.set_flipcb(MAKE_CB(led_panel_white_ct.ready()));
  led_panel_red.begin(LED_PANEL_RED);
  led_panel_red.set_setcb(MAKE_CB(sd_writecommand.get_data()));

  // Communications
  std::vector<Comms*> all_comms = { &wireless, &serial, &sdcomms };
  
  wireless.attach_input_block(wireless_writecommand, COMMAND_AUXDAQ_SDWRITE);
  
  Comms::multiple_attach_output_block(lds_pedal1, LDS_PEDAL_MM, all_comms);
  Comms::multiple_attach_output_block(ts, TIME_AUXDAQ_US, all_comms);
  Comms::multiple_attach_output_block(sd_writecommand, FLAG_AUXDAQ_SDWRITE, all_comms);
  
  edge_detect.attach_input_block(wireless_writecommand, EDGE_RISING);
  edge_detect.attach_input_block(btn_panel, EDGE_FALLING);
  edge_detect.set_cb([](){
    sd_writecommand.flip();
    sdcomms.begin();
  });
  sdcomms.attach_writecommand_block(sd_writecommand);
}

void loop() {
  // put your main code here, to run repeatedly:
  lds_pedal1.update();
  ts.update(); 

  led_onboard.update();
  led_panel_white.update();
  led_panel_red.update();

  edge_detect.update();

  wireless.update();
  sdcomms.update();
  
  if(serial_timer.ready(ts.get_data())){
    btn_panel.update();
    if(DEBUG_PRINTING && USE_SERIAL){
      Serial.print("TIME:   "); Serial.println(ts.get_data());
      Serial.print("PEDAL1:  "); Serial.println(lds_pedal1.get_data());
      Serial.print("BUTTON: "); Serial.println(btn_panel.get_data());
      Serial.print("WRITE:  "); Serial.println(sd_writecommand.get_data());
      Serial.println();
    }
  } 
  if(DEBUG_PRINTING && USE_SERIAL) {
    serial.update();
  }
}
