//#include "Dyno.h"

//HE sensor
#include <SpeedSensor.h>
#include <SDWrite.h>
#include <Sensor.h>
#include <Porter4QD.h>

unsigned __exidx_start;
unsigned __exidx_end;

//SDWrite sd(BUILTIN_SDCARD);
UARTComms uart(115200, Serial);

uint32_t prev_time = micros();
bool led_state = 0;

//Load Cell
#include <Wire.h>

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

//LDS_PIN AND MOTOR_PIN NEED NUMBERS
#define LDS_PIN 14
#define LED_PIN 13
#define MOTOR_PIN 0

//Output Shock Dyno
NAU7802 shock_dyno_force;
//Input Shock Dyno
StateSensor tare_scale;
//Input motor speed
StateSensor motor_speed;
//Output LDS
LDS lds(LDS_PIN, 200);
//Motor Controller
Porter4QD motor_control(MOTOR_PIN, 92 , 1.64);

void setup() {
  //Serial.begin(9600);
  //while(!Serial);
  
  Wire.begin(); 
  Wire.setClock(400000); //Qwiic Scale is capable of running at 400kHz if desired

  pinMode(LED_PIN, OUTPUT);

  if(shock_dyno_force.begin() == false)
  {
    //Serial.println("Scale not detected. Please check wiring. Freezing...");
    digitalWrite(LED_PIN, HIGH);    
  }

  shock_dyno_force.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  shock_dyno_force.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel 
  shock_dyno_force.setGain(NAU7802_GAIN_16);


  uart.begin();
  uart.attach_output_sensor(shock_dyno_force, FORCE_SHOCKDYNO_LBS);
  uart.attach_input_sensor(tare_scale, COMMAND_TARE_LOAD_CELL);
  uart.attach_input_sensor(motor_speed, COMMAND_MOTOR_SPEED);
  uart.attach_output_sensor(lds, LDS_SHOCKDYNO_MM);
  
}

void loop() {
  
  if(abs(micros() - prev_time) > 250000){
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    prev_time = micros();
  }

  if (tare_scale.get_state()){ //Tare the scale
    //Serial.println("Taring the scale");
    shock_dyno_force.calculateZeroOffset();
  }

  if (motor_speed.get_state() == 0){
    motor_control.kill();
  }
  motor_control.setSpeed(motor_speed.get_state());
  
  uart.update();
}
