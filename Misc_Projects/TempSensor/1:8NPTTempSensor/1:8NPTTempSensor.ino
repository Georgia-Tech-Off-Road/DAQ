#define TEENSY_PORT A0
enum TEMP_UNIT  {CELSIUS, FAHRENHEIT, KELVIN, RAW};

// Select tempurature unit here
TEMP_UNIT temp_unit = CELSIUS;

float temp, temp_c, raw;
char temp_char;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TEENSY_PORT, INPUT_PULLUP);
  switch(temp_unit) {
    case CELSIUS:
      temp_char = 'C';
      break;
    case FAHRENHEIT:
      temp_char = 'F';
      break;
    case KELVIN:
      temp_char = 'K';
      break;
    case RAW:
      temp_char = 'R';
      break;
    default:
      temp_char = 'C';
      break;
  }
  delay(100);
}

void loop() {
  raw = analogRead(TEENSY_PORT);
  temp_c = exp(.003 * ((raw - 1024) * -1)) * 7 - 30;
  

  Serial.print("Temperature is: ");
  
  switch(temp_unit) {
    case CELSIUS:
      temp = temp_c;
      break;
    case FAHRENHEIT:
      temp = temp_c * 2 + 32;
      break;
    case KELVIN:
      temp = temp_c + 273;
      break;
    case RAW:
      temp = raw;
      break;
    default:
      temp_unit = CELSIUS;
      break;
  }
  Serial.print(temp);
  Serial.println(temp_char);
  delay(10);
}
