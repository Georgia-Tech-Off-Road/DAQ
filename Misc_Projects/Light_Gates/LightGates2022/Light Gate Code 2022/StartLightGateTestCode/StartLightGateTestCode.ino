#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

uint8_t pin_output = 3;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(15000);
  sensor.startContinuous(15);
  Serial.println("new program");

  pinMode(pin_output, OUTPUT);
  digitalWrite(pin_output, LOW);
}

void loop()
{
  Serial.println(String(millis())+","+String(sensor.read()));
  if (sensor.read() < 2)
  {
    digitalWrite(pin_output, HIGH);
  }
  delay(1000);
  digitalWrite(pin_output, LOW);
}
