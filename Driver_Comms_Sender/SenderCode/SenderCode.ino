/* -------- SenderCode.ino File -------- *
 *
 * This programs the board used to send messages
 * to the driver using a series of switches.
 * 
 * Last updated: 1/26/2023
 * By: Nikola Rogers
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


/* -- Library Inclusions -- */
// Communication Libraries (all libraries in GTORHardwareLibaries>Comms)
#include "SerialComms.h"   // Teensy <-> PC
#include "WirelessComms.h" // Teensy<->XBEE <-......-> XBEE<->Teensy
#include "UARTComms.h"     // Teensy <-> Teensy
//#include "SDComms.h"       // Teensy -> SD Card
#include "BlockId.h"       // Block Ids that are available for sensors

// Sensor Libraries (most valid options in GTORHardwareLibraries>Sensors,
// some are also in GTORHardwareLibraries>ExternalLibraries)
#include "TimeSensor.h"

// Utility Libraries (all libraries in GTORHardwareLibaries>Utility)
#include "ClockTimer.h"

// Control Libraries (all libraries in GTORHardwareLibraries>ControlLibraries)
#include "LEDControl.h"

/* -- Pin Definitions and Constants -- */
// Change this definitions to change the mode that it operates in (only one should be uncommented at a time)
// 0 = Receives data from the XBee
// 1 = Sends data to the XBee
// 14 = Connected to Ex2 Switch
// 15 = Connected to Ex1 Switch
// 16 = Connected to Go Switch
// 17 = Connected to Pit Switch
// 18 = Connected to Stop Switch
//#define OPERATION_MODE 1 // Send data over serial to DAATA application

#define SD_CONNECTED false // Change this to true if you want to use an SD Card

// If using aux daq then use the aux daq pin definition file
// #define TEENSY_LED_PIN 13 // Don't use if using SPI sensors


/* -- Object Creation -- */
// Communication Objects
SerialComms serial(Serial);
//#if SD_CONNECTED
//SDComms sdcomm(BUILTIN_SDCARD);
//#endif

// Sensor Objects
// TimeSensor time_sensor;

// Utility Libraries
// ClockTimerf debug(2); // Print debug messages at 2 Hz

// Control Libraries
LEDControl teensy_led(TEENSY_LED_PIN, 1); // Blink Teensy LED at 1 Hz



/* -------- SETUP --------
 *  
 * This section of the code occurs once
 * Set up the hardware
 *  - Specify pin modes
 *  - Set up specific sensors
 *  - Initialize any communication protocols being used
 */

void setup() {    
  serial.begin();
  serial.attach_output_block(time_sensor, TIME_GENERIC);

  pinMode(14, INPUT); //setup for pins connected to switches
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);

//  #if SD_CONNECTED
//  sdcomm.begin(NULL);
//  sdcomm.attach_output_block(time_sensor, TIME_GENERIC);
//  #endif
  
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
 
void loop() {
  uint32_t current_time = micros(); // Measure time for clock timer object update cycles

  #if 
  
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

//  // Debug
//  #elif OPERATION_MODE == 2
//  serial.update_monitor();
//  #elif OPERATION_MODE == 3
//  if(debug.ready(current_time)){
//    Serial.println("Debug message...");
//  }
//  #endif
}
