

#include <OctoWS2811.h>

// define colors
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PINK 0xFF1088
#define ORANGE 0xE05800
#define WHITE 0xFFFFFF
class LED
{
private:
    OctoWS2811 *leds;

public:
    LED(OctoWS2811 _leds, int _ledsPerStrip);
    void SetStrip(int speed);
};

