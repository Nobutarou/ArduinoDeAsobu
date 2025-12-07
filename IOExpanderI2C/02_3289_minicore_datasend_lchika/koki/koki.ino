#include <Wire.h>

void setup() {
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pinMode(13,OUTPUT);
}

void loop() {
}

void receiveEvent(int howMany) {
  unsigned char c;
  //(void)howMany;  // cast unused parameter to void to avoid compiler warning
  while (Wire.available()) { // loop through all but the last
    c = Wire.read(); // receive byte as a character
  }

  if(c==0){
    digitalWrite(13,HIGH);
  } else {
    digitalWrite(13,LOW);
  }
  Serial.begin(9600);
  Serial.println(c);
  Serial.end();
}
