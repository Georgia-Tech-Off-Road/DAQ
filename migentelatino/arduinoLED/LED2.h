
#include <OctoWS2811.h>
#include <SpeedSensor.h>
#include "Arduino.h"

// define colors
// #define RED 0x990000
// #define GREEN 0x009900
// #define BLUE 0x000099
// #define YELLOW 0x999900
// #define PINK 0x990844
// #define ORANGE 0x702E00
// #define WHITE 0xFFFFFF
// #define PINread 2

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

// Speed PPR and Speed
#define SPEED_ENGINE_PPR 30
#define SPEED_ENGINE 16
#define MAX_RPM 20000
class LED
{
private:
    SpeedSensor *speedSensor;
    OctoWS2811 *leds;
    int ledsPerStrip;
    double speed;

public:
    LED(SpeedSensor *_speedSensor, OctoWS2811 *_leds, int _ledsPerStrip)
    {
        speedSensor = _speedSensor;
        ledsPerStrip = _ledsPerStrip;
        leds = _leds;
        // OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);
    }
    void SetStrip(int color)
    {
        speed = speedSensor->get_speed();
        int numLedsOn = ((double) speed /(double) MAX_RPM)  * (double)ledsPerStrip;
        for (int i = 0; i < ledsPerStrip; i++) {
          if (i <= numLedsOn){
            leds->setPixel((uint32_t)i, color);
          } else {
            leds->setPixel(((uint32_t)i), 0);
          }
          
        }
        leds->show();
        // for (int i = 0; i < ledsPerStrip; i++)
        // {
        //     if ((double) speed /(double) MAX_RPM >= (double) i / (double)ledsPerStrip)
        //         leds->setPixel((uint32_t)i, RED);
        // }
    }
};
