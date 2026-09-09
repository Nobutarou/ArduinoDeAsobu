void setup() {
  // ここは面倒なので任せる
  pinMode(PIN_PA7,OUTPUT);
}

void loop() {
  //PIN_PA7 を High
  PORTA.OUT = PORTA.OUT | 0b10000000;
  delay(1000);
  //PIN_PA7 を LOW
  PORTA.OUT = PORTA.OUT & 0b01111111;
  delay(1000);
  
}

