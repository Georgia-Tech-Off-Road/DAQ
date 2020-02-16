//Time of flight test receiving end
uint16_t incomingByte = 0;
uint16_t incomingByte2 = 0;
uint16_t actualByte = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(500);
  Serial.print("Initialized");

}

void loop() {
  if(Serial1.available()){
    incomingByte = Serial1.read();
    while(!Serial1.available()){
      delay(1);
    }
    incomingByte2 = Serial1.read();
    actualByte = (incomingByte << 8) || incomingByte2;
    
    Serial.println(actualByte);
  }
}
