/* -------- GTOR Data Collection Template .ino File -------- *
 *
 * This file gives an outline for creating an .ino for
 * a Teensy microcontroller that will work with the
 * GTOR comms utility.
 * 
 * Last updated: 02/02/2022
 * By: Andrew Hellrigel
 */



/* -------- START --------
 *  
 * All the code before Setup
 * File inclusions
 *  - Include all the libraries that you want to use in your code
 * Pin definitions and constants
 *  - Define any constants and pins to make code more readable
 * Object and variable creation
 *  - Create all of the objects you want to use in your code
 *  - Create any global variables needed throughout the code
 */

// TIRE RADIUS IN INCHES
#define TIRE_RADIUS 22
#define GEARBOX_RATIO 1

// --- Only have one uncommented at a time --- //
#define RUN
//#define CALIBRATE
//#define TEST

/* -- Library Inclusions -- */
// Communication Libraries (all libraries in GTORHardwareLibaries>Comms)
#include "SerialComms.h"   // Teensy <-> PC
#include "WirelessComms.h" // Teensy<->XBEE <-......-> XBEE<->Teensy
#include "UARTComms.h"     // Teensy <-> Teensy
#include "SDComms.h"       // Teensy -> SD Card
#include "BlockId.h"       // Block Ids that are available for sensors

// Sensor Libraries (most valid options in GTORHardwareLibraries>Sensors,
// some are also in GTORHardwareLibraries>ExternalLibraries)
#include "TimeSensor.h"
#include <SpeedSensor.h>

// Utility Libraries (all libraries in GTORHardwareLibaries>Utility)
#include "ClockTimer.h"

// Control Libraries (all libraries in GTORHardwareLibraries>ControlLibraries)
#include "LEDControl.h"
#include <ServoControl.h>
#include <TLC5952.h>
#include "DashDial.h"
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>

#include <Block.h>
#include <SPI.h>
#include <Wire.h>


/* -- Pin Definitions and Constants -- */
// Change this definitions to change the mode that it operates in (only one should be uncommented at a time)
// 1 = Send data over serial to DAATA application
// 2 = Send debug information as print statements
// 3 = Send debug information automatically collected from sensors to serial monitor
#define OPERATION_MODE 0 // Send data over serial to DAATA application

#define SD_CONNECTED false // Change this to true if you want to use an SD Card

// If using aux daq then use the aux daq pin definition file
#define TEENSY_LED_PIN 13 // Don't use if using SPI sensors

// OLED Screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


/* -- Object Creation -- */
// Communication Objects
SerialComms serial(Serial);
#if SD_CONNECTED
SDComms sdcomm(BUILTIN_SDCARD);
#endif

// Sensor Objects
TimeSensor time_sensor;
  //PPR, pin1, pin2 = 255, flag
SpeedSensor engine_rpm_1(1, 6, 255);
SpeedSensor engine_rpm_2(22, 22, 255);
  //SpeedSensor secondary_rpm(20, 23, 255);
SpeedSensor secondary_rpm(20, 23, 255);
uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

// Utility Libraries
ClockTimerf debug(2); // Print debug messages at 2 Hz

// Control Libraries
LEDControl teensy_led(TEENSY_LED_PIN, 1); // Blink Teensy LED at 1 Hz
WirelessComms wireless(Serial2);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TLC5952 r_driver(12, 11, 10);
TLC5952 l_driver(3, 2, 4);
ServoControl r_servo(9, 270);
ServoControl l_servo(5, 270);
DashDial l_dash(l_driver, l_servo, 0, 400, 0, 500, 10, 265);
DashDial r_dash(r_driver, r_servo, 0, 4000, 0, 500, 10, 265);

/* -------- SETUP --------
 *  
 * This section of the code occurs once
 * Set up the hardware
 *  - Specify pin modes
 *  - Set up specific sensors
 *  - Initialize any communication protocols being used
 */

void setup() {    
  serial.begin(2000000);
  serial.attach_output_block(time_sensor, TIME_DASH_US);

  #if SD_CONNECTED
  sdcomm.begin(NULL);
  sdcomm.attach_output_block(time_sensor, TIME_DASH_US);
  #endif

  wireless.begin(115200);
  wireless.attach_output_block(secondary_rpm, SPEED_SECONDARY30_RPM);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  l_dash.begin();
  r_dash.begin();

  pinMode(17, INPUT);
  pinMode(33, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(29, OUTPUT);

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  Serial2.begin(GPSBaud);
  
  delay(100); // Good to delay for a bit just to allow hardware to initialize
}



/* -------- LOOP --------
 *  
 * This section of the code runs repeatedly
 * Collect data
 *  - Update sensor objects to read data from them
 * Control objects
 *  - Use functions to control different objects (i.e. tell a servo to go to a specific position or tell LED to turn on/off)
 * Communicate
 *  - Read/write data to different devices you are communicating with
 */

uint32_t timer = micros();
 
void loop() {
//  Serial.println(micros() - timer);
//  timer = micros();
    #ifdef RUN
    l_dash.set(secondary_rpm.get_data().speed);
    displaySpeed(secondary_rpm.get_data().speed);
    r_dash.set(secondary_rpm.get_data().speed);
    secondary_rpm.update();
    Serial.print(secondary_rpm.get_data().speed);
    Serial.print("\n");
    l_dash.update();
    r_dash.update();
    // Read speed sensors and update dials
//    static uint32_t prev_change = 0;
//    static uint32_t prev_update = 0;
//    static int16_t var = 0;
//    uint32_t t = micros();
//    if(abs(t - prev_change) >= 1000){
//        float in_per_min = secondary_rpm.get_data().speed * 2 * 3.1415 * TIRE_RADIUS / GEARBOX_RATIO;
//        float mph = (in_per_min * 60.0) / (12.0 * 5280.0);
//        uint16_t mph_10 = (uint16_t) (mph * 10);
//        
//   // Serial.println(mph);
//        r_dash.set(engine_rpm_2.get_data().speed);
//        l_dash.set(mph_10);
//        displaySpeed(mph_10);
//        prev_change = t;
//    }
//    if(abs(t - prev_update) >= 10000){
//        l_dash.update();
//        r_dash.update();
//        prev_update = t;
//        Serial.print("\nEngine Speed 1  (RPM): ");
//        Serial.println(engine_rpm_1.get_data().speed);
//        Serial.print("Engine Speed 2  (RPM): ");
//        Serial.println(engine_rpm_2.get_data().speed);
//        Serial.print("Secondary Speed (RPM): ");
//        Serial.println(secondary_rpm.get_data().speed);
//        Serial.print("\nEngine Position 1: ");
//        Serial.println(engine_rpm_1.get_data().position);
//        Serial.print("Engine Position 2: ");
//        Serial.println(engine_rpm_2.get_data().position);
//        Serial.print("Secondary Position: ");
//        Serial.println(secondary_rpm.get_data().position);
//    }
    #endif
    #ifdef TEST
    static uint32_t prev_update = 0;
    static uint32_t delay_update = 0;
    static uint16_t delay_state = 0;
//    timer = micros();
 //   printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
 //   printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
 //   printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
 //   Serial.print(" \n");
 //   Serial.println(micros() - timer);
 //   timer = micros();
//    Serial.println(engine_rpm_2.get_data().speed);
    if(abs(micros() - prev_update) >= 10000){
      l_dash.update();
      r_dash.update();
      prev_update = micros();
    }
    if(abs(micros() - delay_update) >= 20000){
      if(delay_state > 400){
        delayMicroseconds(2000000);
        delay_state = 0;
      }
      else if (delay_state == 1){
        delayMicroseconds(2000000);
      }
      if (delay_state <= 400){
        l_dash.set(delay_state);
        r_dash.set(delay_state*10);
      //  timer = micros();
        displaySpeed(delay_state);
      //  Serial.println(micros() - timer);
        delay_state++;
      }
      delay_update = micros();
    }

//    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
//    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
//    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
//    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
//    printInt(gps.location.age(), gps.location.isValid(), 5);
//    printDateTime(gps.date, gps.time);
//    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
//    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
//    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
//    printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
//
//    printInt(gps.charsProcessed(), true, 6);
//    printInt(gps.sentencesWithFix(), true, 10);
//    printInt(gps.failedChecksum(), true, 9);
//  Serial.println();
    
    #endif
    #ifdef CALIBRATE
    l_dash.calibrate_servo(3);
    r_dash.calibrate_servo(3);
    #endif
  
  uint32_t current_time = micros(); // Measure time for clock timer object update cycles
  
  // Update Sensors
  time_sensor.update();

  // Update Control Objects
  teensy_led.update();

  // Update Communication Utilities
  #if SD_CONNECTED
  sdcomm.update();
  #endif
  #if OPERATION_MODE == 1
  serial.update();

  // Debug
  #elif OPERATION_MODE == 2
  serial.update_monitor();
  #elif OPERATION_MODE == 3
  if(debug.ready(current_time)){
    Serial.println("Debug message...");
  }
  #endif

  wireless.update();
}

void displaySpeed(uint32_t displayText) {
  display.clearDisplay();

//  display.setTextSize(2);      // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Top-left corner is (0, 0)
//  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  display.println("Speed: " + String(displayText));

  display.display();
  //delay(2000);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while(Serial2.available())
      gps.encode(Serial2.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

//static void printFloat(float val, bool valid, int len, int prec)
//{
//  if (!valid)
//  {
//    while (len-- > 1)
//      Serial.print('*');
//    Serial.print(' ');
//  }
//  else
//  {
//    Serial.print(val, prec);
//    int vi = abs((int)val);
//    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
//    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
//    for (int i=flen; i<len; ++i)
//      Serial.print(' ');
//  }
//}
//
//static void printInt(unsigned long val, bool valid, int len)
//{
//  char sz[32] = "*****************";
//  if (valid)
//    sprintf(sz, "%ld", val);
//  sz[len] = 0;
//  for (int i=strlen(sz); i<len; ++i)
//    sz[i] = ' ';
//  if (len > 0) 
//    sz[len-1] = ' ';
//  Serial.print(sz);
//}
//
//static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
//{
//  if (!d.isValid())
//  {
//    Serial.print(F("********** "));
//  }
//  else
//  {
//    char sz[32];
//    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
//    Serial.print(sz);
//  }
//  
//  if (!t.isValid())
//  {
//    Serial.print(F("******** "));
//  }
//  else
//  {
//    char sz[32];
//    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
//    Serial.print(sz);
//  }
//
//  printInt(d.age(), d.isValid(), 5);
//}
//
//static void printStr(const char *str, int len)
//{
//  int slen = strlen(str);
//  for (int i=0; i<len; ++i)
//    Serial.print(i<slen ? str[i] : ' ');
//}
