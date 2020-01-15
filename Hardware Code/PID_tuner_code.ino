//DEFINITIONS ------------------------------------------------------------------------------------
float setpoint;
float pos; //the input
float output;
float DC;

#define pwm_pin1 3
#define pwm_pin2 5
#define setpnt_pin A1
#define fdbck_pin A0

float curr_err;
float prev_err;
float curr_time;
float prev_time;
float elapsed_time;
String dataString;

float K[3]; //[Kp, Ki, Kd]

float p_err = 0;
float i_err = 0;
float i_raw_err = 0;
float d_err = 0;

void setup() {
Serial.begin(115200); //baudrate is 115200
//READ IN PID PARAMETERS FROM MATLAB --------------------------------------------------------------
int param_counter = 0;
while(param_counter <3){
  param_counter ++;
  float parameter = serialReader();
  K[param_counter] = parameter;
  byte param_flag = 100;
  Serial.write(param_flag);
}
Serial.flush();
//INITIALISE PID VARIABLES ------------------------------------------------------------------------
prev_time = millis(); //initialising a 'previous' time value
  setpoint = 500; //setting first setpoint value to be about half way, this only matters for a split second before a command is received
prev_err = setpoint - analogRead(fdbck_pin); //initialising a 'previous' error value
}

//READ IN SETPOINT FUNCTION COMMAND ---------------------------------------------------------------
int iteration = 1;

void loop() {
//READ IN SETPOINT COMMAND FROM MATLAB ------------------------------------------------------------
  byte setpnt = Serial.read(); //at each instant, MATLAB has a setpoint command
  setpoint = map((float)setpnt, 0, 255, 40, 1000); //clipped to 40-1000 safe range
//EXECUTE PID ALGORITHM ---------------------------------------------------------------------------
  //second, read the actuator position (on 0-1023 scale)
  pos = analogRead(fdbck_pin);
  //third, compare setpoint and position to get error
    //error negative when LA MORE extended than setpoint demands
  curr_err = setpoint - pos;
  //now, apply PID function
  p_err = K[1]*curr_err; //proportional component
    curr_time = millis();
    elapsed_time = curr_time - prev_time;
  i_raw_err = i_raw_err + (curr_err*elapsed_time);
  i_err = K[2]*i_raw_err;//integral component
  d_err = K[3]*((curr_err - prev_err)/elapsed_time); //derivative component
  //output is the sum of the error functions
    //it is a PWM command in the 0-1023 range
    //if it is positive, then the position is less than the setpoint and it should extend the LA
    //it is in a sense proportional to the gap (error)
  output = p_err + i_err + d_err;
  //to transform this into a usable duty cycle, map to signed 8 bit domain
  DC = map(output, -1023, 1023, -255, 255);
//IMPLEMENT PID COMMAND ---------------------------------------------------------------------------
  runActuator(DC);
  //update time and error
  prev_time = curr_time;
  prev_err = curr_err;
//PRINT DATA TO MATLAB ----------------------------------------------------------------------------
//if(iteration == 1){
//  dataString += "$";
//}
//iteration ++;
dataString += "$" + (String)pos + "," + (String)setpoint + "," + (String)curr_time + "," + (String)curr_err + "$";
Serial.println(dataString);
//PID REFRESH INTERVAL ----------------------------------------------------------------------------
  delay(15);
}

void runActuator(float DC){
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

float serialReader(){
//reads in a packet of 6 bytes from MATLAB that encode a float number
//returns the number sent by MATLAB. Must be used in conjuction with the packetWriter function in MATLAB.
float actualNumber;
float f_sign = 1;
while(Serial.available() <6){
  delay(20);
}
unsigned long rawNum = 0;

byte sign = Serial.read();
if(sign == 0){
  f_sign = -1;
}
byte exp10 = Serial.read();
float f_exp10 = (float)exp10;

byte idx1 = Serial.read();
byte idx2 = Serial.read();
byte idx3 = Serial.read();
byte idx4 = Serial.read();

byte arr[4] = {idx1, idx2, idx3, idx4};

  rawNum += arr[3] << 24;
  rawNum += arr[2] << 16;
  rawNum += arr[1] << 8;
  rawNum += arr[0];
float ten = 10;
float powTen = pow(ten, f_exp10);

actualNumber = f_sign*(rawNum/powTen);
return actualNumber;
}
