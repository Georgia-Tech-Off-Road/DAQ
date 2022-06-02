 /* -- Library Inclusions -- */
// Communication Libraries (all libraries in GTORHardwareLibaries>Comms)
#include "SerialComms.h"   // Teensy <-> PC
#include "WirelessComms.h" // Teensy<->XBEE <-......-> XBEE<->Teensy

// Sensor Libraries (most valid options in GTORHardwareLibraries>Sensors,
// some are also in GTORHardwareLibraries>ExternalLibraries)
#include "TimeSensor.h"
#include "DigitalSensor.h"
#include "WT901.h"
#include "Neo6M.h"

// Utility Libraries (all libraries in GTORHardwareLibaries>Utility)
#include "ClockTimer.h"
#include "EdgeDetector.h"

// Control Libraries (all libraries in GTORHardwareLibraries>ControlLibraries)
#include "LEDControl.h"
#include "DashDial.h"

#define TEENSY_LED_PIN 13
#define CMPS_TO_MPH 0.022369363

/* -- Object Creation -- */
// Communication Objects
SerialComms serial(Serial);
WirelessComms wireless(Serial1);

// Sensor Objects
TimeSensor time_sensor;
WT901 imu(Serial5);
Neo6M gps(Serial2);

// Utility Libraries
ClockTimerf dash_update(1000);
ClockTimerf print_update(100);

EdgeDetector edge_detect;
DigitalSensor btn;

// Control Libraries
LEDControl teensy_led(TEENSY_LED_PIN, 1); // Blink Teensy LED at 1 Hz
TLC5952 r_driver(12, 11, 10);
TLC5952 l_driver(3, 2, 4);
ServoControl r_servo(9, 270);
ServoControl l_servo(5, 270);
DashDial l_dash(l_driver, l_servo, 0, 400, 0, 500, 10, 265);
DashDial r_dash(r_driver, r_servo, 0, 4000, 0, 500, 10, 265);
#define REAL 0
#define FAKE 1
#define SWEEP 2
uint8_t state = FAKE;

void setup() {    
  serial.begin(230400);
  wireless.begin(230400);
  
  std::vector<Comms*> all_comms = { &wireless, &serial };
  Comms::multiple_attach_output_block(time_sensor, TIME_DASH_US, all_comms);
  Comms::multiple_attach_output_block(imu, DASHBOARD_IMU_WT901, all_comms);
  Comms::multiple_attach_output_block(gps, DASHBOARD_GPS_SENSOR, all_comms);

  imu.begin(WT901::B4800);
  gps.begin(57600);
  l_dash.begin();
  r_dash.begin();

  teensy_led.begin();
  
  btn.begin(6, INPUT_PULLUP);
  edge_detect.attach_input_block(btn, EDGE_FALLING);
  edge_detect.set_cb([](){ state = (state + 1)%3; });
}


void loop() {
  time_sensor.update();
  btn.update();
  imu.update();
  edge_detect.update();
  gps.update();
  static float mph10;

//  switch(state) {
//  case REAL:
    if (dash_update.ready(time_sensor.get_data())) {
       mph10 = sqrt(gps.get_data().ecefv[0]*gps.get_data().ecefv[0] + 
                          gps.get_data().ecefv[1]*gps.get_data().ecefv[1] + 
                          gps.get_data().ecefv[2]*gps.get_data().ecefv[2]) * 10 * CMPS_TO_MPH;
       l_dash.set(mph10);
//       r_dash.set();
       l_dash.update();
    }
//    break;
//  default:
//  case FAKE:
//    break;
//  case SWEEP:
//    break;
//  }

//  if(print_update.ready(time_sensor.get_data())) {
//    Serial.print("state: "); Serial.println(state);
//    Serial.print("btn: "); Serial.println(btn.get_data());
//    Serial.print("acc_mag: "); Serial.println(imu.get_data().acc_mag);
//    Serial.print("ecefp: ");Serial.print(gps.get_data().ecefp[0]);
//    Serial.print(", ");     Serial.print(gps.get_data().ecefp[1]);
//    Serial.print(", ");     Serial.println(gps.get_data().ecefp[2]);
//    Serial.print("mph10: "); Serial.println(mph10 );
//    Serial.println();
//  }
  teensy_led.update();
  serial.update();
  wireless.update();
}
