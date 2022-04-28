#include <ADS8688.h>
#include <LDS.h>
#include <LoadCell.h>

ADS8688 ads(10);
//ADS8688 ads(33);

LDS<uint8_t> lds(150);
LoadCell load_cell;

void setup() {
  Serial.begin(9600);
  ads.attach_sensor(lds, 0);
  ads.attach_sensor(load_cell, 1);
  
  SPI.begin();
  ads.begin();
}
void loop() {
  ads.update_sensors();

  Serial.print(lds.get_data());
  Serial.print(",");
  Serial.println(load_cell.get_data());

  delay(15);
}
