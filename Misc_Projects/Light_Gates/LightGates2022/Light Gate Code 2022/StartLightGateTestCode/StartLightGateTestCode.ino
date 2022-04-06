#include <Wire.h>
#include <VL53L1X.h>

VL53L1X Distance_Sensor;

uint8_t pin_output = 3;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  Distance_Sensor.setTimeout(500);
  if (!Distance_Sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  Distance_Sensor.setDistanceMode(VL53L1X::Long);
  Distance_Sensor.setMeasurementTimingBudget(33000);
  Distance_Sensor.startContinuous(33);

  pinMode(pin_output, OUTPUT);
  digitalWrite(pin_output, LOW);
}

void loop()
{
  Distance_Sensor.read();
 // Serial.println(Distance_Sensor.ranging_data.range_mm);
 // Serial.println(digitalRead(pin_output));
  digitalWrite(pin_output, Distance_Sensor.ranging_data.range_mm < 1000 ? HIGH : LOW);
}
