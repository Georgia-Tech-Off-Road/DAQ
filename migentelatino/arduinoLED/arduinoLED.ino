
//include libraries and header files
#include <OctoWS2811.h>
#include <Arduino.h>
#include "LED2.h"
#include <stdint.h>



//set up
const int numPins = 2;
byte pinList[numPins] = { 14};
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
LED ledClass(&speedSensor, &leds, ledsPerStrip);
void setup() {
  leds.begin();
  leds.show();
  speedSensor.begin(SPEED_ENGINE);
}

void loop() {
  speedSensor.update();
  ledClass.SetStrip();
}
