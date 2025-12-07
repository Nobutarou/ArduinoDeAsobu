// pin definition
// GPIOA-0 = D9 
#define _a0 9 
#define _a1 10 
#define _a2 11 
#define _a3 12 
#define _a4 13 
#define _a5 A0 
#define _a6 A1 
#define _a7 A2 
#define _b0 A3 
#define _b1 2 
#define _b2 3 
#define _b3 4 
#define _b4 5 
#define _b5 6 
#define _b6 7 
#define _b7 8 

// register
#define _gpioa 0x12
#define _gpiob 0x13

// koki 7bit address
#define _address 0b0100000
#include <Wire.h>

void setup() {
  randomSeed(analogRead(0));
  Wire.begin(); 
  Wire.beginTransmission(_address); 
  Wire.write(_gpioa);
  Wire.write(0b00000000);
  Wire.endTransmission();   
  Wire.beginTransmission(_address); 
  Wire.write(_gpioa);
  Wire.write(0b00000000);
  Wire.endTransmission();   
}

void loop() {
  Wire.beginTransmission(_address); 
  Wire.write(_gpioa);
  Wire.write(random(256));
  Wire.endTransmission();   

  Wire.beginTransmission(_address); 
  Wire.write(_gpiob);
  Wire.write(random(256));
  Wire.endTransmission();   
  delay(1000);
}

