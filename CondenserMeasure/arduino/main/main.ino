#define _R 10000.0; // 実測値

#include <Arduino.h>

const char PULSE_PIN = 2;
const char DIGITAL_READ_PIN = 3;
const char ANALOG_READ_PIN = A5;

void setup()
{
  Serial.begin(9600);
  Serial.println("setup");
  pinMode( PULSE_PIN , OUTPUT);
  digitalWrite(PULSE_PIN, LOW);

  // PIC と違い レジスタ名bits.ビット名での操作はできない。
  // & は 1&1=1, 1&0=0, 0&1=0, 0&0=0 
  // まず上位 5ビットを保持したまま、下位3ビットをクリア
  ADCSRA = ADCSRA & 0b11111000;

  // 第2 ビットを 1 に変える。
  // | or は 1&1=1, 1&0=1, 0&1=1, 0&0=0 だから、第2 ビット以外は元の値が保持される
  ADCSRA = ADCSRA | 0b00000010;

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
  
  Serial.println(c,16);
  digitalWrite(PULSE_PIN, LOW);
}
