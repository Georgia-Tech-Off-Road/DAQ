
#include <OctoWS2811.h>
#include <SpeedSensor.h>
#include "Arduino.h"

// define colors
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PINK 0xFF1088
#define ORANGE 0xE05800
#define WHITE 0xFFFFFF
#define PINread 2

// Speed PPR and Speed
#define SPEED_ENGINE_PPR 30
#define SPEED_ENGINE 17
#define MAX_RPM 100
class LED
{
private:
    SpeedSensor *speedSensor;
    OctoWS2811 *leds;
    int ledsPerStrip;
    int speed;

public:
    LED(SpeedSensor *_speedSensor, OctoWS2811 *_leds, int _ledsPerStrip)
    {
        speedSensor = _speedSensor;
        leds = _leds;
    }
    void SetStrip()
    {
        speed = speedSensor->get_speed();
        for (int i = 0; i < ledsPerStrip; i++)
        {
            if (speed / MAX_RPM >= i / ledsPerStrip)
                leds->setPixel((uint32_t)i, RED);
        }
    }
};
