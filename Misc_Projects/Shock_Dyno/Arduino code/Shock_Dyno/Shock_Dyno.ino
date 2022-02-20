#include <Porter4QD.h>
#include <LoadCell.h>
#include <LDS.h>
#include <SerialComms.h>
#include <Block.h>
#include <BlockId.h>
#include <ADS8688.h>
#include <ClockTimer.h>

#define LDS_PIN 1
#define LOAD_CELL_PIN 2
#define LED_PIN 13
#define MOTOR_PIN 2
#define MOTOR_ENABLE_PIN 4
#define MOTOR_KILL_RELAY_PIN 5

#define MOTOR_CONTROL_SCALE 2.32
#define MOTOR_CONTROL_OFFSET 7.73

SerialComms serial_comms(Serial);

ADS8688 ads(10);
Porter4QD motor_control(MOTOR_PIN, MOTOR_ENABLE_PIN, MOTOR_CONTROL_SCALE, MOTOR_CONTROL_OFFSET);

//input blocks
Block<bool> tare_scale;
Block<uint8_t> motor_speed;
Block<bool> motor_enable;
Block<float> load_cell_scale;

//output blocks
LoadCell load_cell;
LDS<uint8_t> lds(150);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_KILL_RELAY_PIN, OUTPUT);

  SPI.begin();
  serial_comms.begin(115200);

  serial_comms.attach_input_block(tare_scale, COMMAND_TARE_LOAD_CELL);
  serial_comms.attach_input_block(motor_speed, COMMAND_MOTOR_SPEED);
  serial_comms.attach_input_block(motor_enable, COMMAND_MOTOR_ENABLE);
  serial_comms.attach_input_block(load_cell_scale, COMMAND_SCALE_LOAD_CELL);
  serial_comms.attach_output_block(load_cell, FORCE_SHOCKDYNO_LBS);
  serial_comms.attach_output_block(lds, LDS_SHOCKDYNO_MM);

  ads.attach_sensor(lds, LDS_PIN);
  ads.attach_sensor(load_cell, LOAD_CELL_PIN);

  digitalWrite(MOTOR_KILL_RELAY_PIN, HIGH);
}

void loop() {

  //Tare the scale
  if (tare_scale.get_data()) {
    load_cell.tare();
  }

  if (motor_speed.get_data() == 0){
    motor_control.kill();
  }

  if (motor_enable.get_data() == 0) {
    motor_control.kill();
    //motor_control.setSpeed(0);
    digitalWrite(MOTOR_KILL_RELAY_PIN, LOW);
    //Serial.println("bruh");
  }

  if (motor_enable.get_data()) {
    motor_control.setSpeed(motor_speed.get_data());
  }

  ads.update_sensors();

  tare_scale.update();
  motor_speed.update();
  motor_enable.update();
  load_cell_scale.update();

//  load_cell.update();
//  lds.update();
  
  load_cell.set_scale(load_cell_scale.get_data());
  serial_comms.update();
}
