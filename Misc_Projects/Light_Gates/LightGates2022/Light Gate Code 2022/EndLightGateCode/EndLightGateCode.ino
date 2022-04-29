#include <Wire.h>
#include <VL53L1X.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

VL53L1X Distance_Sensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t pin_input = 3;
uint8_t x = 0;
uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t final_time = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  pinMode(pin_input, INPUT_PULLUP);
  x = 0;
//  x = 1;

  Distance_Sensor.setTimeout(500);
  if (!Distance_Sensor.init())
  {
    Serial.println("Failed to initialize VL53L1X Distance_Sensor!");
    while (1);
  }
  
  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 33 ms for medium and long distance mode
  Distance_Sensor.setDistanceMode(VL53L1X::Long);
  Distance_Sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  Distance_Sensor.startContinuous(50);

  displaySpeed(69);
  
  delay(100);
}

void loop()
{
//  Serial.println(digitalRead(pin_input));
//  delay(25);
  Distance_Sensor.read();
  while(x == 0)
  {
    if(digitalRead(pin_input) == LOW)
    {
      start_time = millis();
      displaySpeed(420);
      x = 1;
    }  
  }
  if (Distance_Sensor.ranging_data.range_mm < 1000)
  {
    end_time = millis();
    Serial.println(start_time);
    Serial.println(end_time);
    final_time = end_time - start_time;
    display.clearDisplay();
    displaySpeed(final_time);
    Serial.println("Final time: " + String(final_time/1000.0));
    delay(5000);
    x = 0;
    displaySpeed(x);
  }
}

void displaySpeed(uint32_t displayText) {
  display.clearDisplay();

  display.setCursor(0, 0);     // Top-left corner is (0, 0)

  if (displayText == 0)
  {
    display.println("Ready for next time!");
  }
  else if(displayText == 69)
  {
    display.print("Waiting to start!");    
  }
  else if(displayText == 420)
  {
    display.print("In progress...");
  }
  else
  {
    display.println("Time: " + String(displayText/1000.0));
  }

  display.display();
  delay(2000);
}
