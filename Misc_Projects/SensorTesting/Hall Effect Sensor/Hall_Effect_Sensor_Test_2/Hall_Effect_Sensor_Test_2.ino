/* FreqMeasureMulti - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
 *
 * This example code is in the public domain.
 */
#include <FreqMeasureMulti.h>


FreqMeasureMulti freq1;


void setup() {
  Serial.begin(57600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(10);
  //Serial.println("FreqMeasureMulti Begin");
  delay(10);
  freq1.begin(6);
}

unsigned long sum1=0;
float curr_speed = 0;
int count1=0;
int toneWheelTeeth = 22;
unsigned long prev_tm = micros();
unsigned long curr_tm;
int wait_time = 20; //ms

void loop() {
  //If a tooth has passed, read the time since the last tooth passed
  if (freq1.available()) {
    sum1 = sum1 + freq1.read();
    count1++;
  }
  //If 5ms has elapsed
  if (abs(micros() - prev_tm) > (wait_time * 1000)) { //absolute value in case mircros() wraps back to 0 which occurs ~70 mins from start time
    //If there has been any readings in past 20ms (wait_time). If not, speed is < 150 rpm
    if (count1 > 0) {
      curr_speed = freq1.countToFrequency(sum1 / count1) / toneWheelTeeth * 60;
    }
    //If there weren't any readings, then wait another 20ms (wait_time) because a speed < 150 rpm could potentially read as 0 rpm
    else {
      int i = 0;
      while(not(freq1.available())){
        Serial.println(curr_speed);
        delay(wait_time);
        //if there is no measurements it will set the curr_speed to the max speed that it could possibly be without taking any measurements assuming
        //it is less than the previous speed
        if((60 / (toneWheelTeeth * ((2 + i) * wait_time))) < curr_speed){
          curr_speed = 60 / (toneWheelTeeth * ((2 + i) * wait_time));
        }
        i++;
      }
      curr_speed = freq1.countToFrequency(freq1.read()) / toneWheelTeeth * 60;
    }
    if (curr_speed < 1) {
      curr_speed = 0;
    }
    Serial.println(curr_speed);
    sum1 = 0;
    count1 = 0;
    prev_tm = micros();
  }
}
