#include <Wire.h>

void setup() {
  Wire.begin(); 
}

unsigned char i = 0;

void loop() {

  Wire.beginTransmission(1); 
  Wire.write(i);
  Wire.endTransmission();   
  i = i^1; //反転

  Serial.begin(9600);
  Serial.println(i);
  Serial.end();
  delay(220);
}

