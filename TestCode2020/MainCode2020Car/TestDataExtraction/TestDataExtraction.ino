void setup() {
  Serial.begin(9600);
  delay(500);
  int i = 3;
  byte incomingBytes[4] = {0};
  incomingBytes[0] = 0xF0;
  incomingBytes[1] = 0x00;
  incomingBytes[2] = 0x00;
  incomingBytes[3] = 0x0F;

  Serial.println(incomingBytes[0]);
  Serial.println(incomingBytes[1]);
  Serial.println(incomingBytes[2]);
  Serial.println(incomingBytes[3]);
  Serial.println(0xF000000F);
  Serial.println((uint32_t)incomingBytes[0] << 24 | (uint32_t)incomingBytes[1] << 16 | (uint32_t)incomingBytes[2] << 8 | (uint32_t)incomingBytes[3]);
  Serial.println((240 << 24) | (0 << 16) | (0 << 8) | 15);

}

void loop() {
  // put your main code here, to run repeatedly:

}
