#include "../OctoWS2811/OctoWS2811.h"
#include <SpeedSensor.h>
#include <Arduino.h>
#include "arduinoLED.h"
#include <stdint.h>
#include <rpcndr.h>
#include <stddef.h>
#include <xtr1common>
#include <yvals_core.h>

class LED
{
private:
    int speed;
    OctoWS2811 *leds;
    int ledsPerStrip;
    int color;

public:
    LED(OctoWS2811 _leds, int _ledsPerStrip)
    {
        leds = &_leds;
        ledsPerStrip=_ledsPerStrip;
    }
    void SetStrip(int speed, int MAX_RPM)
    {
        int numLedsOn = ((double)speed / (double)MAX_RPM) * (double)ledsPerStrip;
        if (speed / (double)MAX_RPM < 0.2)
        {
            color = BLUE;
        }
        else if (speed / (double)MAX_RPM < 0.4)
        {
            color = GREEN;
        }
        else if (speed / (double)MAX_RPM < 0.6)
        {
            color = YELLOW;
        }

        else if (speed / (double)MAX_RPM < 0.8)
        {
            color = ORANGE;
        }
        else
        {
            color = RED;
        }
        for (int i = 0; i < ledsPerStrip; i++)
        {
            if (i <= numLedsOn)
            {
                leds->setPixel((uint32_t)i, color);
            }
            else
            {
                leds->setPixel(((uint32_t)i), 0);
            }
        }
        leds->show();
    }
};