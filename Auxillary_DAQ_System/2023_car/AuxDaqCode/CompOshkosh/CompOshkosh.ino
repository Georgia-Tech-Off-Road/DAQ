#include "CompOshkosh.h"

#define DEBUG_PRINTING 1
#define USE_SERIAL 1

// Communications
WirelessComms wireless(Serial1);
SerialComms serial(Serial);
SDComms sdcomms;

// Sensors
TeensyADC tadc;
ADS8688 adc(ADC3);
LDS<float> lds_pedal1(LDS_PEDAL_STROKE);
LDS<float> lds_pedal2(LDS_PEDAL_STROKE);
LDS<float> lds_pedal3(LDS_PEDAL_STROKE);
LDS<float> lds_pedal4(LDS_PEDAL_STROKE);
BrakePressureSensor brake_front;
BrakePressureSensor brake_rear;
GPSSensor gps(Serial1); // Serial doesn't matter, only getting data from dashboard 

SpeedSensor speed_engine(SPEED_ENGINE_PPR);
SpeedSensor speed_secondary(SPEED_SECONDARY_PPR);
IMUSensor imu;
TimeSensor ts;

// Dashboard
std::vector<uint8_t> dashboard_packet;
std::vector<BaseBlock*> dashboard_sensors = { &speed_engine, &speed_secondary, &gps };

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

void read_dashboard() {
  while (DASHBOARD_SERIAL.available()) {
    dashboard_packet.push_back(DASHBOARD_SERIAL.read());
    for (int i = 0; i < 7; ++i) {
      if (dashboard_packet[dashboard_packet.size() - 8 + i] != 0xff) {
        return;
      }
    }
    if (dashboard_packet[dashboard_packet.size() - 1] == 0xf0) {
      uint8_t *data = dashboard_packet.data();
      int idx = 0;
      for (BaseBlock *block : dashboard_sensors) {
        block->unpack(data + idx);
        idx += block->get_packlen();
      }
      dashboard_packet.clear();
    }
  }
}

void setup() {
  // Comms
  serial.begin(230400);
  wireless.begin(230400);
  sdcomms.set_sending_period(1000);

  DASHBOARD_SERIAL.begin(9600);

  // Sensors
  SPI.begin();
  adc.begin();
  pinMode(ADC1, OUTPUT); digitalWrite(ADC1, HIGH);
  pinMode(ADC2, OUTPUT); digitalWrite(ADC2, HIGH);
  adc.attach_sensor(lds_pedal1, LDS_PEDAL1);
  adc.attach_sensor(lds_pedal2, LDS_PEDAL2);
  adc.attach_sensor(lds_pedal3, LDS_PEDAL3);
  adc.attach_sensor(lds_pedal4, LDS_PEDAL4);
  //adc.attach_sensor(brake_front, BRAKE_FRONT);
  //adc.attach_sensor(brake_rear , BRAKE_REAR );

  speed_engine.begin(SPEED_ENGINE);
  speed_secondary.begin(SPEED_SECONDARY);
  imu.begin(IMU);

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
  Comms::multiple_attach_output_block(lds_pedal2, LDS_PEDAL_MM, all_comms);
  Comms::multiple_attach_output_block(lds_pedal3, LDS_PEDAL_MM, all_comms);
  Comms::multiple_attach_output_block(lds_pedal4, LDS_PEDAL_MM, all_comms);
  //Comms::multiple_attach_output_block(brake_front, PRESSURE_FRONTBRAKE_PSI, all_comms);
  //Comms::multiple_attach_output_block(brake_rear,  PRESSURE_REARBRAKE_PSI , all_comms);
  Comms::multiple_attach_output_block(speed_engine, SPEED_2021CAR_ENGINE600_RPM, all_comms);
  Comms::multiple_attach_output_block(speed_secondary, SPEED_2021CAR_SECONDARY30_RPM, all_comms);
  Comms::multiple_attach_output_block(imu,  IMU_SENSOR, all_comms);
  Comms::multiple_attach_output_block(ts, TIME_AUXDAQ_US, all_comms);
  Comms::multiple_attach_output_block(sd_writecommand, FLAG_AUXDAQ_SDWRITE, all_comms);
  Comms::multiple_attach_output_block(gps, GPS_SENSOR, all_comms);
  
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
  lds_pedal2.update();
  lds_pedal3.update();
  lds_pedal4.update();
  //brake_front.update();
  //brake_rear.update();
  //speed_engine.update();
  //speed_secondary.update();
  imu.update();
  ts.update(); 

  led_onboard.update();
  led_panel_white.update();
  led_panel_red.update();

  edge_detect.update();

  wireless.update();
  sdcomms.update();

  read_dashboard();
  
  if(serial_timer.ready(ts.get_data())){
    btn_panel.update();
    if(DEBUG_PRINTING && USE_SERIAL){
      Serial.print("TIME:   "); Serial.println(ts.get_data());
      Serial.print("PEDAL1:  "); Serial.println(lds_pedal1.get_data());
      Serial.print("PEDAL2:  "); Serial.println(lds_pedal2.get_data());
      Serial.print("PEDAL3:  "); Serial.println(lds_pedal3.get_data());
      Serial.print("PEDAL4:  "); Serial.println(lds_pedal4.get_data());
      //Serial.print("FBRAKE: "); Serial.println(brake_front.get_data());
      //Serial.print("RBRAKE: "); Serial.println(brake_rear.get_data());
      Serial.print("ENGINE: "); Serial.println(speed_engine.get_data().speed);
      Serial.print("SECOND: "); Serial.print(speed_secondary.get_data().speed); Serial.print(", "); Serial.println(speed_secondary.get_data().position);
      Serial.print("IMU:    "); imu.printall(); Serial.println();
      Serial.print("BUTTON: "); Serial.println(btn_panel.get_data());
      Serial.print("WRITE:  "); Serial.println(sd_writecommand.get_data());
      Serial.println();
    }
  } 
  if(DEBUG_PRINTING && USE_SERIAL) {
    serial.update();
  }
}
