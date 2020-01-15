#include <FreqMeasureMulti.h>

class HallEffectSpeedSensorX {
private:

    uint8_t _inputPin;
    uint16_t _currSpeed;
    uint16_t * _readings;
    uint32_t _prevTime;
    uint32_t _intervalTime;
    uint8_t _toneWheelTeeth;
    uint16_t _averagingAmount;
    uint16_t _intervalLength; //How often _currSpeed is updated in microseconds (limited by processor speed)
    uint16_t _index;
    uint16_t _count;
    uint32_t _sum;
    uint16_t _timeCounter;
    FreqMeasureMulti _freq;
    
public:
  /**
   * Sets up the digital out pins and the PID if specified.
   */
  HallEffectSpeedSensorX(int inputPin, uint8_t toneWheelTeeth, uint16_t intervalLength = 0,uint16_t averagingAmount = 1){
    _inputPin = inputPin;
    _toneWheelTeeth = toneWheelTeeth; //Multiply by 2 when using FREQMEASUREMULTI_ALTERNATE mode
    _averagingAmount = averagingAmount;
    _intervalTime = micros();
    _intervalLength = intervalLength;
    _currSpeed = 0;
    _readings = new uint16_t[averagingAmount];
    _count = 0;
    _sum = 0;
    _index = 0;
    _timeCounter = 1;
  }

  ~HallEffectSpeedSensorX(){
    delete _readings;
  }

  void begin(){
    _freq.begin(_inputPin); //, FREQMEASUREMULTI_ALTERNATE);
    Serial.println("Interval Length: " + String(_intervalLength));
    Serial.println("Averaging Amount: " + String(_averagingAmount));
    for (int i = 0; i < _averagingAmount; i++) {
      _readings[i] = 0;
    }
    delay(20);
  }
  
  void updateSensor(){
    if (_freq.available()){
      _sum += _freq.read();
      _count++;
    }
    
    if (abs(micros() - _intervalTime) > _intervalLength){
      _intervalTime = micros();
      //If any readings calculate a new curr_speed
      if (_count > 0) {
      _currSpeed = _freq.countToFrequency(_sum / _count) * 60 / _toneWheelTeeth; // (1 tooth / s) * (60s / 1 min) * (1 rev / x teeth) = rpm
      _timeCounter = 1;
      } 
      //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
      else if(60000000 / (_toneWheelTeeth * (_intervalLength * _timeCounter)) < _currSpeed) {
        _currSpeed = 60000000 / (_toneWheelTeeth * (_intervalLength * _timeCounter));
        _timeCounter++;
      }
      else{_timeCounter++;}
  
      _sum = 0;
      _count = 0;
      _readings[_index] = _currSpeed;
  
      //Increment or reset index
      if(_index == _averagingAmount - 1){
        _index = 0;
      }
      else{
       _index++;
      }
    }
  }
  
  int getSpeed(){
    int sum = 0;
    for (int i = 0; i < _averagingAmount; i++) {
      sum += _readings[i];
    }
    return sum / _averagingAmount;
//    return _currSpeed;
  }
};

HallEffectSpeedSensorX Sensor(6, 20, 50, 200);
uint64_t timer = micros();

void setup(){
  Serial.begin(115200);
  delay(100);
  Sensor.begin();
}

void loop(){
  Sensor.updateSensor();
  if (micros() - timer > 20000){
    Serial.println(Sensor.getSpeed());
    timer = micros();
  }
  
}
