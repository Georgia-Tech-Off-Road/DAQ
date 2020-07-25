//Time of flight test sending end
long initTime = 0;
int incomingByte = 0;
long TOF = 0;
long timeout = 2000000; //2 Seconds in microseconds

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

}

void loop() {
  Serial1.write(1);
  initTime = micros();
  while(!Serial1.available() && ((micros() - initTime) < timeout)){
    
  }
  if(Serial1.available()){
    TOF = (micros() - initTime) / 2;
    incomingByte = Serial1.read();
    Serial.print("Time of Flight: ");
    Serial.println(TOF);
  }
  else{
    Serial.println("No data received: Timeout occured");
  }
  delay(500);
  while(Serial1.available()){
    Serial1.read();
  }
}
