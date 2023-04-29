#include "SerialComms.h"
#include "WirelessComms.h"

WirelessComms wireless(Serial1);
SerialComms serial(Serial);
Block<bool> wireless_writecommand;

void setup() {
  // put your setup code here, to run once:
  serial.begin(230400);
  wireless.begin(230400);

  serial.attach_input_block(wireless_writecommand, COMMAND_AUXDAQ_SDWRITE);
  wireless.attach_output_block(wireless_writecommand, COMMAND_AUXDAQ_SDWRITE);
  

}
void loop() {
  // put your main code here, to run repeatedly:
  wireless.update();
  Serial.print("SD Write:   "); Serial.println(wireless_writecommand.get_data());
  serial.update();
}
