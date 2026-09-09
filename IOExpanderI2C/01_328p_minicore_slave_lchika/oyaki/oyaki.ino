#include <Wire.h>

void setup() {
  Wire.begin(); 
}

byte x = 0;

void loop() {
  Wire.beginTransmission(1); 

  // 何も送る必要はないのだが、送らないということが許されるのか知らないので送る。
  // 試したところ不要だった
  //Wire.write(1);
  Wire.endTransmission();   
  delay(220);
}

