#include "UARTComms.h"

UARTComms uart1;
HallEffectSpeedSensor s;

void setup() {
    uart1.attach_input_sensor(s, table);
    std::vector<SensorInfo> ignores = {ENINE, ese}
}

void loop() {

}