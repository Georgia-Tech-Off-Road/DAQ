#include "SevenSegment.h"


SevenSegment::SevenSegment(Adafruit_TLC5947 &driver) : 
    _driver(driver) {
    _digit[0] = _digit[1] = _digit[2] = 0;
}

void SevenSegment::set_digit(uint8_t segment, uint8_t digit){
    if(segment > 2 || digit > 9) return;
    _digit[segment] = digit;
}

void SevenSegment::set_number(uint16_t number){
    set_digit(0, number % 10);
    set_digit(1, number / 10 % 10);
    set_digit(2, number / 100 % 10);
}

void SevenSegment::set_dp(uint8_t segment, uint8_t dp){
    if(segment > 2 || dp > 1) return;
    _dp[segment] = dp;
}

void SevenSegment::begin(){
    _driver.begin();
}

void SevenSegment::update() {
    for(uint8_t i = 0; i < 24; ++i)
        _driver.setPWM(i, 0);

    switch(_digit[0]){
    case 0: //  1,  2,  3, 21, 22, 23
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM( 3, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 1: //  1, 23
        _driver.setPWM( 1, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 2: // 22, 23, 20,  3,  2
        _driver.setPWM( 2, 1023);
        _driver.setPWM( 3, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 3: // 22, 23, 20,  1,  2
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 4: // 21, 23, 20,  1
        _driver.setPWM( 1, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 5: // 22, 21, 20,  1,  2
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(22, 1023);
        break;
    case 6: // 22, 21, 20,  3,  1,  2
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM( 3, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(22, 1023);
        break;
    case 7: // 22, 23,  1
        _driver.setPWM( 1, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 8: // 22, 21, 23, 20,  3,  1,  2
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM( 3, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    case 9: // 22, 21, 23, 20,  1,  2
        _driver.setPWM( 1, 1023);
        _driver.setPWM( 2, 1023);
        _driver.setPWM(20, 1023);
        _driver.setPWM(21, 1023);
        _driver.setPWM(22, 1023);
        _driver.setPWM(23, 1023);
        break;
    }

    switch(_digit[1]){
    case 0: //  5,  6,  7, 17, 18, 19
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM( 7, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 1: //  5, 19
        _driver.setPWM( 5, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 2: // 18, 19, 16,  7,  6
        _driver.setPWM( 6, 1023);
        _driver.setPWM( 7, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 3: // 18, 19, 16,  5,  6
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 4: // 17, 19, 16,  5
        _driver.setPWM( 5, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 5: // 18, 17, 16,  5,  6
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(18, 1023);
        break;
    case 6: // 18, 17, 16,  7,  5,  6
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM( 7, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(18, 1023);
        break;
    case 7: // 18, 19,  5
        _driver.setPWM( 5, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 8: // 18, 17, 19, 16,  7,  5,  6
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM( 7, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    case 9: // 18, 17, 19, 16,  5,  6
        _driver.setPWM( 5, 1023);
        _driver.setPWM( 6, 1023);
        _driver.setPWM(16, 1023);
        _driver.setPWM(17, 1023);
        _driver.setPWM(18, 1023);
        _driver.setPWM(19, 1023);
        break;
    }

    
    switch(_digit[2]){
    case 0: //  9, 10, 11, 13, 14, 15
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(11, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 1: //  9, 15
        _driver.setPWM( 9, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 2: // 14, 15, 12, 11, 10
        _driver.setPWM(10, 1023);
        _driver.setPWM(11, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 3: // 14, 15, 12,  9, 10
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 4: // 13, 15, 12,  9
        _driver.setPWM( 9, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 5: // 14, 13, 12,  9, 10
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(14, 1023);
        break;
    case 6: // 14, 13, 12, 11,  9, 10
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(11, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(14, 1023);
        break;
    case 7: // 14, 15,  9
        _driver.setPWM( 9, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 8: // 14, 13, 15, 12, 11,  9, 10
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(11, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    case 9: // 14, 13, 15, 12,  9, 10
        _driver.setPWM( 9, 1023);
        _driver.setPWM(10, 1023);
        _driver.setPWM(12, 1023);
        _driver.setPWM(13, 1023);
        _driver.setPWM(14, 1023);
        _driver.setPWM(15, 1023);
        break;
    }

    _driver.setPWM(0, _dp[0]*1023);
    _driver.setPWM(4, _dp[1]*1023);
    _driver.setPWM(8, _dp[2]*1023);

    _driver.write();
}