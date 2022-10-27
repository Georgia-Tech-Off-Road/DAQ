#define TEENSY_PORT A0

// Select tempurature unit here
// 'C', 'F', 'K', 'R' (Debug)
char temp_unit = 'C';

float temp, temp_c;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TEENSY_PORT, INPUT_PULLUP);
  delay(100);
}

void loop() {
  temp_c = exp(.003 * ((analogRead(TEENSY_PORT) - 1024) * -1)) * 7 - 26;
  
  switch(temp_unit) {
    case 'C':
      temp = temp_c;
      break;
    case 'F':
      temp = temp_c * 2 + 32;
      break;
    case 'K':
      temp = temp_c + 273;
      break;
    case 'R':
      temp = analogRead(TEENSY_PORT);
      break;
    default:
      // If no temp_unit is set or an invalid value is set, it will default to 'C'
      temp_unit = 'C';
      break;
  }
  
  Serial.print("Temperature is: ");
  Serial.print(temp);
  Serial.println(temp_unit);
  delay(10);
}
