#include <FreqMeasureMulti.h>

FreqMeasureMulti freq1a;

unsigned long sum1=0;
float curr_speed = 0;
int count1=0;
unsigned long prev_tm = micros();
const int toneWheelTeeth = 22 * 2; //Add *2 when using exclusive or gate between quad signals
const int wait_time = 20; //ms
const int trailing_average_amount = 4;
float readings[trailing_average_amount];
int readIndex = 0;
int i = 0;

void setup() {
  Serial.begin(57600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(20);
  freq1a.begin(6);
  //Initialize all elements in readings array to 0
  for (int thisReading = 0; thisReading < trailing_average_amount; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  //If a tooth has passed, read the clock cycles since the last tooth passed
  if (freq1a.available()) {
    sum1 = sum1 + freq1a.read();
    count1++;
  }
  
  //If 20ms (wait time) has elapsed
  if (abs(micros() - prev_tm) > (wait_time * 1000)) { //absolute value in case mircros() wraps back to 0 which occurs ~70 mins from start time
    
    //If any readings calculate a new curr_speed
    if (count1 > 0) {
      curr_speed = freq1a.countToFrequency(sum1 / count1) / toneWheelTeeth * 60;
      i = 0;
    }    
    //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
    else if((60 / (toneWheelTeeth * ((2 + i) * wait_time))) < curr_speed) {
      curr_speed = 60 / (toneWheelTeeth * ((2 + i) * wait_time));
      i++;
    }
    //Else assume curr_speed stays the same

    readings[readIndex] = curr_speed;
    readIndex = changeIndex(readIndex, trailing_average_amount);
    sum1 = 0;
    count1 = 0;
    Serial.println(arrayAverage(readings, trailing_average_amount));
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
