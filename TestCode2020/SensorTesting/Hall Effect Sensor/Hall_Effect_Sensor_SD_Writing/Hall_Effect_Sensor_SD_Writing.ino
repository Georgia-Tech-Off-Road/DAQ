#include <FreqMeasureMulti.h>
#include <SD.h>

FreqMeasureMulti freq;
File aFile;
String fileName = "EngineDyno1FlywheelTest1.TXT";
uint32_t sum = 0;
float curr_speed = 0;
uint16_t count = 0;
uint32_t prev_tm = micros();
uint32_t start_tm = micros();
uint32_t collection_tm = 25 * 1000000; //in microseconds
const int toneWheelTeeth = 20;
const float wait_time = 0.095; //ms (subtract about 5us from desired for code execution)
uint16_t reading = 0;
int i = 0;


void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(20);
  freq.begin(6);
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialization failed!");
    return;
  }
  else{Serial.println("initialization successful!");}
  aFile = SD.open("EDFT3.TXT", FILE_WRITE);
}

void loop() {
  //If a tooth has passed, read the clock cycles since the last tooth passed
  if (freq.available()) {
    sum = sum + freq.read();
    count++;
  }
  //Close file after set amount of time
  if (abs(micros() - start_tm) > collection_tm){
    aFile.close();
  }

  if (abs(micros() - prev_tm) > (wait_time * 1000)) { //absolute value in case mircros() wraps back to 0 which occurs ~70 mins from start time
    //If any readings calculate a new curr_speed
    if (count > 0) {
      curr_speed = freq.countToFrequency(sum / count) / toneWheelTeeth * 60;
      i = 1;
    }    
    //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
    else if(60000 / (toneWheelTeeth * i * wait_time) < curr_speed) {
      curr_speed = 60000 / (toneWheelTeeth * i * wait_time);
      i++;
    }
    else{i++;} 

    reading = curr_speed;
    sum = 0;
    count = 0;
    
//    aFile.print(micros() - start_tm);
//    aFile.print(',');
//    aFile.println(reading);
    Serial.print(micros() - start_tm);
    Serial.print(',');
    Serial.println(reading);      
    prev_tm = micros();
  }
}
