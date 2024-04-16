
//include libraries and header files
#include <OctoWS2811.h>
#include <Arduino.h>
#include "LED2.h"
#include <stdint.h>



//set up
const int numPins = 1;
byte pinList[numPins] = { 22};
const int ledsPerStrip = 144;
const int bytesPerLED = 3;
//buffers
DMAMEM int displayMemory[ledsPerStrip * numPins * bytesPerLED / 4];
int drawingMemory[ledsPerStrip * numPins * bytesPerLED / 4];
//set configuration as GRB and 800kHz
const int config = WS2811_GRB | WS2811_800kHz;
//create OctoWS2811
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);
//write in order the colors that want to be displayed, only with five colors
int colors[] = { BLUE, GREEN, YELLOW, ORANGE, RED };
//class for LED
int maxSignal=200;
//speedSensor
SpeedSensor speedSensor(SPEED_ENGINE_PPR);
//TESTING XD
int color=0;
LED ledClass(&speedSensor, &leds, ledsPerStrip);
void setup() {
  leds.begin();
  leds.show();
  speedSensor.begin(SPEED_ENGINE);
}

void loop() {
  speedSensor.update();
  Serial.println(speedSensor.get_speed());
  if(speedSensor.get_speed()/(double) MAX_RPM<0.2)
  {
    color=BLUE;
  }
  else if(speedSensor.get_speed()/(double) MAX_RPM<0.4)
  {
    color=GREEN;
  }
  else if(speedSensor.get_speed()/(double) MAX_RPM<0.6)
  {
    color=YELLOW;
  }
  
  else if(speedSensor.get_speed()/(double) MAX_RPM<0.8)
  {
    color=ORANGE;
  }
  else
  {
    color=RED;
  }
  
  
  ledClass.SetStrip(color);
  // leds.setPixel(1, RED);

}
