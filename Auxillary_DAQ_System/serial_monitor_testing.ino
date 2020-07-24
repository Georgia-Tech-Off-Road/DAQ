#include <PID_v1.h>
double setpoint;
double pos; //the input
double output;
double PIDoutput;
double DC;
double PID_DC;


#define pwm_pin1 3
#define pwm_pin2 5
#define setpnt_pin A1
#define fdbck_pin A0

double curr_err;
double prev_err;
double curr_time;
double prev_time;
double elapsed_time;

double Kp = 3;
double Ki = 0.005;
double Kd = 1;

double p_err = 0;
double i_err = 0;
double i_raw_err = 0;
double d_err = 0;

PID myPID(&pos, &PIDoutput, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
Serial.begin(115200);
//configuring setpoint input (from trimpot)
pinMode(10, OUTPUT);
digitalWrite(10, HIGH); //reference supply for pot

myPID.SetMode(AUTOMATIC); //default configs for PID function

prev_time = millis(); //initialising a 'previous' time value
  setpoint = analogRead(setpnt_pin);
  if(setpoint <50){
    setpoint = 50;
  }
  else if(setpoint >980){
    setpoint =980;
  }
prev_err = setpoint - analogRead(fdbck_pin); //initialising a 'previous' error value
}


void loop() {
  //first, read in the setpoint and clip to safe range
  setpoint = analogRead(setpnt_pin);
  if(setpoint <50){
    setpoint = 50;
  }
  else if(setpoint >980){
    setpoint =980;
  }
  //second, read the actuator position (on 0-1023 scale)
  pos = analogRead(fdbck_pin);
  //third, compare setpoint and position to get error
    //error negative when LA MORE extended than setpoint demands
  curr_err = setpoint - pos;
  //now, apply PID function
  p_err = Kp*curr_err; //proportional component
    curr_time = millis();
    elapsed_time = curr_time - prev_time;
  i_raw_err = i_raw_err + (curr_err*elapsed_time);
  i_err = Ki*i_raw_err;//integral component
  d_err = Kd*((curr_err - prev_err)/elapsed_time); //derivative component
  //output is the sum of the error functions
    //it is a PWM command in the 0-1023 range
    //if it is positive, then the position is less than the setpoint and it should extend the LA
    //it is in a sense proportional to the gap (error)
  output = p_err + i_err + d_err;
  //to transform this into a usable duty cycle, map to signed 8 bit domain
  DC = map(output, -1023, 1023, -255, 255);
//    myPID.Compute();
//    double PIDsign = 1;
//  if (curr_err <0){
//    PIDsign = -1;
//  }
//  DC = PIDsign*PIDoutput;
  runActuator(DC);
  
  prev_time = curr_time;
  prev_err = curr_err;
  
  //serial communication stuff
  byte byte_pos = (byte)map(pos, 0, 1023, 0, 255);
  byte byte_setpnt = (byte)map(setpoint, 0, 1023, 0, 255);
  byte byte_DC = (byte)DC;
  byte byte_DCsign = 1;
  if(DC <0){
    byte_DCsign = 0;
  }
  byte data_arr[4] = {byte_pos, byte_DC, byte_DCsign, byte_setpnt};
  Serial.write(data_arr, 4);
  delay(15);
}

void runActuator(double DC){
  //DC is PWM command from [-255, 255]
  //when negative, retract
  if(DC <0){
    analogWrite(pwm_pin1, 0);
    analogWrite(pwm_pin2, abs(DC));
  }
  //when positive, extend
  else if(DC >0){
    analogWrite(pwm_pin1, abs(DC));
    analogWrite(pwm_pin2, 0);
  }
  else{
    analogWrite(pwm_pin1, 0);
    analogWrite(pwm_pin2, 0);
  }
}
