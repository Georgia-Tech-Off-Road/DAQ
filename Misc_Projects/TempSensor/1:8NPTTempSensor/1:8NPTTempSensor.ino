#define TEENSY_PORT A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TEENSY_PORT, INPUT_PULLUP);
  delay(100);
}

void loop() {
  Serial.print("Temperature is: ");
  Serial.print(analogRead(TEENSY_PORT));
  Serial.println(" degrees celsius");
}
