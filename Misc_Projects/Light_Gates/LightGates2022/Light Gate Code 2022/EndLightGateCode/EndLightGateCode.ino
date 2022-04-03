#include <Wire.h>
#include <VL53L1X.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

VL53L1X sensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t pin_input = 3;
uint8_t x = 0;
uint32_t start_time = 0;
uint32_t end_time = 0;
uint8_t final_time = 0;

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

  pinMode(pin_input, INPUT);
  x = 0;

  delay(100);
}

void loop()
{
  Serial.println(String(millis())+","+String(sensor.read()));
  while(x == 0)
  {
    display.clearDisplay();
    displaySpeed("Ready!");
    if(pin_input == HIGH)
    {
      start_time = millis();
      display.clearDisplay();
      x = 1;
    }  
  }
  if (sensor.read() < 2)
  {
    end_time = millis();
    final_time = end_time - start_time;
    displaySpeed(final_time);
    Serial.println("Final time: " + String(final_time));
    delay(5000);
    x = 0;
  }
}

void displaySpeed(uint32_t displayText) {
  display.clearDisplay();

  display.setCursor(0, 0);     // Top-left corner is (0, 0)
  
  display.println("Time: " + String(displayText));

  display.display();
  delay(2000);
}
