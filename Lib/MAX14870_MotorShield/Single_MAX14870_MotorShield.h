#ifndef SINGLE_MAX14870_MOTORSHIELD
#define SINGLE_MAX14870_MOTORSHIELD

#include <Arduino.h>

class Single_MAX14870_MotorShield
{
  public:
    Single_MAX14870_MotorShield(uint8_t pin_PWM, uint8_t pin_DIR, uint8_t pin_nEN, uint8_t pin_nFAULT);
    void setup();
    void enableDrivers();
    void disableDrivers();
    bool get_fault();   // returns whether a fault has occurred on the motor shield
    void set_speed(int16_t speed);    // sets the speed of the motor (value from -256 to 256);

  private:
    const uint8_t _pin_PWM;
    const uint8_t _pin_DIR;
    const uint8_t _pin_nEN;
    const uint8_t _pin_nFAULT;
};

#endif