void setup() {
  Serial.begin(9600);

}

void loop() {
  for(int i = 0; i < 100; i++){
    Serial.println("Hello");
  }
  delay(1000);

}
