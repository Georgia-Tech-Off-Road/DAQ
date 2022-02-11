#include <ClockTimer.h>

#include <ServoControl.h>
ServoControl servoR(9, 270);
ServoControl servoL(5, 270);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <TLC5952.h>
TLC5952 testR(12, 11, 10);
TLC5952 testL(3, 2, 4);

//#include <TinyGPS.h>
//#include <SoftwareSerial.h>
//TinyGPS gps;
//#define gpsPort Serial2
//char buf[32];

float testSpeed = 1;
uint32_t topSpeed = 40;
uint32_t servoSet = 0;
uint32_t ledSet = 0;

unsigned long bruh;

void setup() {
  // put your setup code here, to run once:
  servoR.begin();
  servoL.begin();

  Serial.begin(115200);
//  gpsPort.begin(9600);
//  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
//  Serial.println("by Mikal Hart");
//  Serial.println();

if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  
  testR.begin();
  testL.begin();
  pinMode(17, INPUT);
  testR.set_control_data(127, 127, 127);
  testL.set_control_data(127, 127, 127);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (float i = 0.0; i <= 40; i += 0.25){
    bruh = micros();
    Serial.println(bruh);
    servoSet = ((float) i / (float) topSpeed) * 270.0;
    servoR.writeDeg(servoSet);
    servoL.writeDeg(servoSet);
    ledSet = (((float) i / (float) topSpeed) * 23.0);
    ledSet = 16777216 - (pow(2, 23 - ledSet));
    testR.set_on_off_data(ledSet);
    testL.set_on_off_data(ledSet);
    testR.write();
    testL.write();
    bruh = micros();
    Serial.println(bruh);
    testdrawtest(i);
  }
  for (float i = 40.0; i >= 0; i -= 0.25){
    servoSet = ((float) i / (float) topSpeed) * 270.0;
    servoR.writeDeg(servoSet);
    servoL.writeDeg(servoSet);
    ledSet = (((float) i / (float) topSpeed) * 23.0);
    ledSet = 16777216 - (pow(2, 23 - ledSet));
    testR.set_on_off_data(ledSet);
    testL.set_on_off_data(ledSet);
    testR.write();
    testL.write();
    testdrawtest(i);
  } 

//  bool newData = false;
//  unsigned long chars;
//  unsigned short sentences, failed;
//  // For one second we parse GPS data and report some key values
//  for (unsigned long start = millis(); millis() - start < 1000;) {
//    while (gpsPort.available()) {
//      char c = gpsPort.read();
//      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
//      if (gps.encode(c)) // Did a new valid sentence come in?
//        newData = true;
//    }
//  }
//
// unsigned long age;
//  if (newData) {
//    float flat, flon;
//    gps.f_get_position(&flat, &flon, &age);
//    Serial.print("LAT=");
//    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
//    Serial.print(" LON=");
//    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
//    Serial.print(" SAT=");
//    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
//    Serial.print(" PREC=");
//    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
//    //GPS mode
//    Serial.print(" Constellations=");
//    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0 : gps.constellations());
//  }
//
//  //satellites in view
//  uint32_t* satz = gps.trackedSatellites();
//  uint8_t sat_count = 0;
//  for(int i=0;i<24;i++) {
//    if(satz[i] != 0) {   //exclude zero SNR sats
//      sat_count++;
//      byte strength = (satz[i]&0xFF)>>1;
//      byte prn = satz[i]>>8;
//      sprintf(buf, "PRN %d: ", prn);
//      Serial.print(buf);
//      Serial.print(strength);
//      Serial.println("dB");
//    }
//  }
//  
//  //date time
//  int year;
//  uint8_t month, day, hour, minutes, second, hundredths;
//  gps.crack_datetime(&year, &month, &day, &hour, &minutes, &second, &hundredths, &age);
//  sprintf(buf,"GPS time: %d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minutes, second);
//  Serial.println(buf);
//
//  gps.stats(&chars, &sentences, &failed);
//  Serial.print(" CHARS=");
//  Serial.print(chars);
//  Serial.print(" SENTENCES=");
//  Serial.print(sentences);
//  Serial.print(" CSUM ERR=");
//  Serial.println(failed);
//  if (chars == 0)
//  Serial.println("** No characters received from GPS: check wiring **");
} // End of main loop()

// Draw characters of the default font
void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 32);     // Top-left corner is (0, 0)
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }

//  display.println(F("Type whatever you want and it'll show up on the screen"));

// Writes "I <3 Wasiq"
  display.write(73);
  display.write(' ');
  display.write(3);
  display.write(' ');
  display.write(87);
  display.write(97);
  display.write(115);
  display.write(105);
  display.write(113);
  display.write(19);
  
  display.display();
  delay(2000);
}

void testdrawtest(uint32_t displayText) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 32);     // Top-left corner is (0, 0)
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  display.println("Speed: " + String(displayText));

  display.display();
  //delay(2000);
}
