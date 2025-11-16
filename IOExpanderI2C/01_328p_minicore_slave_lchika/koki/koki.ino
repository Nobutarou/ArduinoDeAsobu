
#include <Wire.h>

void setup() {
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pinMode(13,OUTPUT);
}

void loop() {
}

void receiveEvent(int howMany) {
  digitalWrite(13,HIGH);
//  (void)howMany;  // cast unused parameter to void to avoid compiler warning
//  while (1 < Wire.available()) { // loop through all but the last
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
//  int x = Wire.read();    // receive byte as an integer
//  Serial.println(x);         // print the integer
}
