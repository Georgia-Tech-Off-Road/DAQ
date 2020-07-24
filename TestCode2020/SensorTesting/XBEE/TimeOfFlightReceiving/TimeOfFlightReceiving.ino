//Time of flight test receiving end
int incomingByte = 0;

void setup() {
  Serial1.begin(9600);

}

void loop() {
  if(Serial1.available()){
    incomingByte = Serial1.read();
    Serial1.write(incomingByte);
  }
}
