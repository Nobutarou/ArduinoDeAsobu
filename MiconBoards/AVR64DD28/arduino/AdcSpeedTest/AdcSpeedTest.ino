#define _AIN PIN_PD1

void setup() {
  char i;
  unsigned long time0;
  unsigned long time1;
  
  Serial.begin(9600);

  Serial.println("ADC_Clock, Duration, Time");

  Serial.print(analogClockSpeed());
  Serial.print(", ");
  Serial.print(getAnalogSampleDuration());
  Serial.print(", ");

  analogReadResolution(12);

  i=0;
  time0=micros();
  while(i<100){
    analogRead(_AIN);
    i++;
  }
  time1=micros();
  Serial.println(time1-time0);

  Serial.print(analogClockSpeed(2000));
  Serial.print(", ");

  analogSampleDuration(0);
  Serial.print(getAnalogSampleDuration());
  Serial.print(", ");

  i=0;
  time0=micros();
  while(i<100){
    analogRead(_AIN);
    i++;
  }
  time1=micros();
  Serial.println(time1-time0);
}

void loop() {
}

