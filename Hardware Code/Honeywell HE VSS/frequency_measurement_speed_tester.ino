#include <FreqMeasure.h>

void setup() {
  Serial.begin(250000); //250K baudrate for superfast transmission
  FreqMeasure.begin();
  pinMode(13, OUTPUT);
  while(Serial.available() == 0){;
  delay(1);
  }
  digitalWrite(13, HIGH);
}

double sum=0;
int count=0;

//uint32_t freq;
uint16_t freq;
//unsigned long curr_tm;
uint16_t period_f;
unsigned long strt_tm = micros();
unsigned long curr_tm;
unsigned long overall_cnt = 0;

void loop() {
  if (FreqMeasure.available()) {
    // average several reading together
      //this is not necessary, strictly speaking, because Nyquist is ~13KHz, well above the tooth frequency of even a 50 tooth wheel at 3800RPM. Still, protective measure.
    sum = sum + FreqMeasure.read();
    count++;
      freq = (uint16_t) FreqMeasure.countToFrequency(sum / count); //converting the CPU time counts to a (Hz) frequency using canned function. Casting to 4-byte, 32 bit word
        Serial.write((byte) freq); //bit shifting 32 bit word into 8 bit words, reverse writing as per FIFO communication protocol
        Serial.write((byte) (freq>> 8));
//        tm2 = micros();
//        period_f = (uint16_t) (1e6/(tm2 - tm1));
//        tm1 = tm2;
//        Serial.write((byte) period_f);
//        Serial.write((byte) (period_f>> 8));
//        Serial.write((byte) (freq>> 16));
//        Serial.write((byte) (freq>> 24));
//        curr_tm = micros(); //recording microseconds since Teensy started (also 32 bit unsigned) ---- overflow after 71 minutes of operation. Either hard reset Teensy at intervals or switch to 64 bit.
//        Serial.write((byte) curr_tm); //same bit-shift and send procedure
//        Serial.write((byte) (curr_tm>> 8));
//        Serial.write((byte) (curr_tm>> 16));
//        Serial.write((byte) (curr_tm>> 24));
      sum = 0; //resetting counter/sum registers
      count = 0;
      overall_cnt++;
    }
  }
