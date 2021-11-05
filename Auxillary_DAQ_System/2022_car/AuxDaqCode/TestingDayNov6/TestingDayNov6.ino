// Communication Libraries
#include "SerialComms.h"
#include "WirelessComms.h"
#include "BlockId.h"

// Sensor Libraries
#include "TeensyADC.h"
#include "SpeedSensor.h"
#include "LDS.h"
#include "IMUSensor.h"

// Utility
#include "ClockTimer.h"


// Pin Definitions and Constants
#define LDS_FL A7
#define LDS_FR A8
#define LDS_BL A9
#define LDS_BR A10
#define LDS_FL_STROKE 200
#define LDS_FR_STROKE 200
#define LDS_BL_STROKE 225
#define LDS_BR_STROKE 225
#define SPEED_PRIMARY 15
#define SPEED_PRIMARY_PPR 600
#define SPEED_SECONDARY 16
#define SPEED_SECONDARY_PPR 30
#define IMU_CS 10
#define LED_PIN 30
#define ADC1_CS 33
#define ADC2_CS 34
#define ADC3_CS 35


// Object Creation
SerialComms serial(Serial);
WirelessComms wireless(Serial1);
TeensyADC tadc;
ClockTimerf led_timer(2);

LDS<uint8_t> lds_fl(LDS_FL_STROKE);
LDS<uint8_t> lds_fr(LDS_FR_STROKE);
LDS<uint8_t> lds_bl(LDS_BL_STROKE);
LDS<uint8_t> lds_br(LDS_BR_STROKE);
SpeedSensor speed_primary(SPEED_PRIMARY_PPR, SPEED_PRIMARY, 255);
SpeedSensor speed_secondary(SPEED_SECONDARY_PPR, SPEED_SECONDARY, 255);
IMUSensor imu(IMU_CS);

bool led_on = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  tadc.attach_sensor(lds_fl, LDS_FL);
  tadc.attach_sensor(lds_fr, LDS_FR);
  tadc.attach_sensor(lds_bl, LDS_BL);
  tadc.attach_sensor(lds_br, LDS_BR);
  
  serial.begin(115200);
  serial.attach_output_block(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  serial.attach_output_block(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  serial.attach_output_block(lds_bl, LDS_BACKLEFTSHOCK_MM);
  serial.attach_output_block(lds_br, LDS_BACKRIGHTSHOCK_MM);
  serial.attach_output_block(speed_primary, SPEED_2021CAR_ENGINE600_RPM);
  serial.attach_output_block(speed_secondary, SPEED_2021CAR_SECONDARY30_RPM);
  serial.attach_output_block(imu, IMU_SENSOR);

  wireless.begin(115200);
  wireless.attach_output_block(lds_fl, LDS_FRONTLEFTSHOCK_MM);
  wireless.attach_output_block(lds_fr, LDS_FRONTRIGHTSHOCK_MM);
  wireless.attach_output_block(lds_bl, LDS_BACKLEFTSHOCK_MM);
  wireless.attach_output_block(lds_br, LDS_BACKRIGHTSHOCK_MM);
  wireless.attach_output_block(speed_primary, SPEED_2021CAR_ENGINE600_RPM);
  wireless.attach_output_block(speed_secondary, SPEED_2021CAR_SECONDARY30_RPM);
  wireless.attach_output_block(imu, IMU_SENSOR);

  pinMode(ADC1_CS, OUTPUT);
  pinMode(ADC2_CS, OUTPUT);
  pinMode(ADC3_CS, OUTPUT);
  digitalWrite(ADC1_CS, HIGH);
  digitalWrite(ADC2_CS, HIGH);
  digitalWrite(ADC3_CS, HIGH);

  imu.begin();
  
  delay(100);
}

void loop() {
  lds_fl.update();
  lds_fr.update();
  lds_bl.update();
  lds_br.update();
  speed_primary.update();
  speed_secondary.update();
  imu.update();
  wireless.update();
  //serial.update();
  
  if(led_timer.ready(micros())) {
    led_on = !led_on;
    digitalWrite(LED_PIN, led_on);
  }  
}
