#define _R 9400.0; // 実測値

#include <Arduino.h>

const int PULSE_PIN = 2;
const int DIGITAL_READ_PIN = 3;
const int ANALOG_READ_PIN = 5;


void setup()
{
  Serial.begin(9600);
  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW);
  Serial.println("setup");
}


void discharge() { 
  pinMode(DIGITAL_READ_PIN, OUTPUT);
  digitalWrite(DIGITAL_READ_PIN, LOW);
  delay(1000);
  pinMode(DIGITAL_READ_PIN, INPUT);
  delay(10);
}

unsigned long charge() {
  digitalWrite(PULSE_PIN, HIGH);
  return micros();
}

void loop() {
  discharge();

  unsigned long time_start = charge();

  while ( analogRead(ANALOG_READ_PIN) < 647)
  {
  }; //1023*64.2=646.5。

  double T = micros() - time_start; // T: 時定数
  double c=T/_R; // [uF]
  
  Serial.println(c);
  digitalWrite(PULSE_PIN, LOW);
  // discharge(); // 2回は不要では？

}
