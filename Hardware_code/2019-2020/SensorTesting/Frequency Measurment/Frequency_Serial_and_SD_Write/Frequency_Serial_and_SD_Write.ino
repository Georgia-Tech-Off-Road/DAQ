#include <FreqMeasureMulti.h>
#include <SD.h>

FreqMeasureMulti freq;
File aFile;
uint32_t sum = 0;
float a_freq = 0;
uint16_t count = 0;
uint32_t start_tm = micros();


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
  aFile = SD.open("FREQS1.TXT", FILE_WRITE);
}

void loop() {
  if (freq.available()) {
    sum = sum + freq.read();
    count++;
  }

  if (count > 0) {
    a_freq = freq.countToFrequency(sum / count);
  }    


  sum = 0;
  count = 0;
  
  aFile.print(micros() - start_tm);
  aFile.print(',');
  aFile.println(reading);
  Serial.println(a_freq);      

}
