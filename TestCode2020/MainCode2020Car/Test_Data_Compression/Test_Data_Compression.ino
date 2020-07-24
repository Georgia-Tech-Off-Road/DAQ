byte d[8] = {0};
uint64_t atime = 0b11111111111111, atime2 = 0;
uint32_t atime3 = 0, atime4 = 0;

void setup() {
  Serial.begin(9600);
  delay(500);
  d[0] = (byte) atime >> 56;
  d[1] = (byte) atime >> 48;
  d[2] = (byte) atime >> 40;
  d[3] = (byte) atime >> 32;
  d[4] = (byte) atime >> 24;
  d[5] = (byte) atime >> 16;
  d[6] = (byte) atime >> 8;
  d[7] = (byte) atime;

  atime2 = ((uint64_t)d[0]) << 56 | ((uint64_t)d[1]) << 48 | ((uint64_t)d[2]) << 40 | ((uint64_t)d[3]) << 32 | ((uint64_t)d[4]) << 24 | ((uint64_t)d[5]) << 16 | ((uint64_t)d[6]) << 8 | ((uint64_t)d[7]);
  atime3 = (uint32_t) atime2 >> 32;
  atime4 = (uint32_t) atime2;
  for(int i = 0; i < 8; i++){
    Serial.println(d[i]);
  }
  Serial.println(atime3);
  Serial.print(atime4);
}

void loop() {
  // put your main code here, to run repeatedly:

}
