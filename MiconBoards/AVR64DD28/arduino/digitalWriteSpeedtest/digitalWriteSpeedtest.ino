void setup() {
  pinMode(PIN_PC0,OUTPUT);
  digitalWrite(PIN_PC0, LOW);

  Serial.begin(9600);
}

void loop() {
  unsigned long time0;
  unsigned long time1;
  unsigned short i;

  i=0;
  time0=micros();
  while(i<1000){
    digitalWrite(PIN_PC0, HIGH);
    i++;
  };
  time1=micros();
  Serial.print("DH1000, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);

  i=0;
  time0=micros();
  while(i<500){
    digitalWrite(PIN_PC0, HIGH);
    digitalWrite(PIN_PC0, LOW);
    i++;
  };
  time1=micros();
  Serial.print("DHL500, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);

  i=0;
  time0=micros();
  while(i<1000){
    digitalWrite(PIN_PC0, LOW);
    i++;
  };
  time1=micros();
  Serial.print("DL1000, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);

  i=0;
  time0=micros();
  while(i<1000){
    // PIN_PC0 を High
    PORTC.OUT = PORTC.OUT | 0b00000001;
    i++;
  };
  time1=micros();
  Serial.print("PH1000, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);

  i=0;
  time0=micros();
  while(i<500){
    // PIN_PC0 を High
    PORTC.OUT = PORTC.OUT | 0b00000001;
    //PIN_PC0 を LOW
    PORTC.OUT = PORTC.OUT & 0b11111110;
    i++;
  };
  time1=micros();
  Serial.print("PHL500, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);

  i=0;
  time0=micros();
  while(i<1000){
    //PIN_PC0 を LOW
    PORTC.OUT = PORTC.OUT & 0b11111110;
    i++;
  };
  time1=micros();
  Serial.print("PL1000, ");
  Serial.println(time1-time0);
  digitalWrite(PIN_PC0, LOW);
}
