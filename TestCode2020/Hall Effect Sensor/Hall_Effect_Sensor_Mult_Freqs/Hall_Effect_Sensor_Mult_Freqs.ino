#include <FreqMeasureMulti.h>

FreqMeasureMulti freq[4];
unsigned long sum[4] = {0,0,0,0};
float curr_speed[4] = {0,0,0,0};
int count[4] = {0,0,0,0};
unsigned long prev_tm = micros();
const int toneWheelTeeth1 = 22;
const int toneWheelTeeth2 = 0;
const int toneWheelTeeth[4] = {toneWheelTeeth1,toneWheelTeeth1,toneWheelTeeth2,toneWheelTeeth2}; //For looping
const int wait_time = 20; //ms
const int trailing_average_amount = 4;
float readings[4][trailing_average_amount];
int readIndex = 0;
int i[4] = {0,0,0,0};

void setup() {
  Serial.begin(57600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(20);
  freq[0].begin(6);
  freq[1].begin(5);
  freq[2].begin(9);
  freq[3].begin(10);
  //Initialize all elements in readings array to 0
  for (int j = 0; j > 4; j++){
    for (int thisReading = 0; thisReading < trailing_average_amount; thisReading++) {
      readings[j][thisReading] = 0;
    }    
  }

}

void loop() {
  //If a tooth has passed, read the clock cycles since the last tooth passed
  for(int j = 0; j < 4; j++){
    if (freq[j].available()) {
      sum[j] = sum[j] + freq[j].read();
      count[j]++;
    }
  }

  //If 20ms (wait time) has elapsed
  if (abs(micros() - prev_tm) > (wait_time * 1000)) { //absolute value in case mircros() wraps back to 0 which occurs ~70 mins from start time

    for(int j = 0; j < 4; j++){
      //If any readings calculate a new curr_speed
      if (count[j] > 0) {
        curr_speed[j] = freq[j].countToFrequency(sum[j] / count[j]) / toneWheelTeeth[j] * 60;
        i[j] = 0;
      }    
      //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
      else if((60 / (toneWheelTeeth[j] * ((2 + i[j]) * wait_time))) < curr_speed[j]) {
        curr_speed[j] = 60 / (toneWheelTeeth[j] * ((2 + i[j]) * wait_time));
        i[j]++;
      }
      //Else assume curr_speed stays the same 
    }

    for(int j = 0; j < 1; j++){
      readings[j][readIndex] = curr_speed[j];
      sum[j] = 0;
      count[j] = 0;
      Serial.print(arrayAverage(readings[j], trailing_average_amount));
      Serial.print('\t');   
    }
    Serial.println();
    readIndex = changeIndex(readIndex, trailing_average_amount);
    prev_tm = micros();
  }
}



float arrayAverage(float measurements[], int arr_len){
  int sum = 0;
  for (int thisReading = 0; thisReading < arr_len; thisReading++) {
    sum += measurements[thisReading];
  }
  return sum / arr_len;
}

int changeIndex(int curr_index, int arr_len){
  if(curr_index == arr_len - 1){
    curr_index = 0;
  }
  else{
    curr_index++;
  }
  return curr_index;
}
