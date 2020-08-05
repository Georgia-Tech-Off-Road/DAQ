double i;
int delta;
uint8_t matrix[64] = {0};
int temp_int;
String temp_string;
bool bool_cont;

struct Sensor_Settings {

  const static uint8_t num_sensors = 3;
  const static uint8_t num_sensorSettings = 4;


  String sensorList[num_sensors - 1][num_sensorSettings - 1] = { {0} };


  inline void setup_sensorList() {
    /* Sensor Setting Key
          0: sensor name
          1: enable bit position
          2: x-axis label
          3:
    */
    sensorList[0][0] = "throttle";
    sensorList[0][1] = "0";
    sensorList[0][2] = "seconds";
    sensorList[0][3] = "x-axis";
    sensorList[1][0] = "one zero";
    sensorList[1][1] = "one one";
    sensorList[1][2] = "one two";
    sensorList[1][3] = "one three";
    sensorList[2][0] = "two zero";
    sensorList[2][1] = "two one";
    sensorList[2][2] = "two two";
    sensorList[2][3] = "two three";
  }





} sensors;

void setup()  {

  Serial.begin(115200);
  i = 0;
  delta = .1;
  Serial.println("testing...");
  sensors.setup_sensorList();
  delay(20);
  setup_MatLabArduinoComms();
  delay(20);
  send_sensorList();
  delay(20);

}




void loop() {
  //  Serial.println(uint8_t(sin(i)*100));
  Serial.println("hello");



  i = i + .1;
  delay(1000);

}


//  Waits until a code has been received from Matlab to proceed
inline void setup_MatLabArduinoComms() {
  while (Serial.readString().trim() != "start_arduinoSetup") {}

  Serial.println("start_setup");
  Serial.println(sensors.num_sensors);
  Serial.println(sensors.num_sensorSettings);

}

inline void send_sensorList() {
  //  bool_cont = false;
  //  while (bool_cont == false) {
  //    if (Serial.readString().trim() == "start_listSend") {
  //      bool_cont = true;
  //    }
  //  }

  while (Serial.readString().trim() != "start_listSend") {}

  Serial.println("begin_sensor");

  Serial.println(sensors.sensorList[0][0]);
  Serial.println(sensors.sensorList[0][1]);
  Serial.println(sensors.sensorList[0][2]);
  Serial.println(sensors.sensorList[0][3]);
  Serial.println(sensors.sensorList[1][0]);
  Serial.println(sensors.sensorList[1][1]);
  Serial.println(sensors.sensorList[1][2]);
  Serial.println(sensors.sensorList[1][3]);
  Serial.println(sensors.sensorList[2][0]);
  Serial.println(sensors.sensorList[2][1]);
  Serial.println(sensors.sensorList[2][2]);
  Serial.println(sensors.sensorList[2][3]);



  //  code for printing that doesn't work and needs to be debugged
  //  for (int n=0; n<=2; n++) {
  //    for (int x=0; x<=3; x++) {
  //      Serial.println(sensors.sensorList[n][x]);
  //    }
  //  }

}
