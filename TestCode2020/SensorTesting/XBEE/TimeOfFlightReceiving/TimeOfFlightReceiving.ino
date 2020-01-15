//Time of flight test receiving end
int incomingByte = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {
  if(Serial.available()){
    incomingByte = Serial.read();
    Serial.write(incomingByte);
  }
}
