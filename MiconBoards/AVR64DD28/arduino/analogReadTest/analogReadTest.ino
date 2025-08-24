#define _AIN PIN_PD6

void setup() {
  Serial.begin(9600);

  analogReadResolution(12);

  Serial.println("ADC_Clock, Duration, adc");

  // なぜか初回はでたらめな数値が出る。
  Serial.print(analogClockSpeed(-1));
  Serial.print(", ");
  Serial.print(getAnalogSampleDuration());
  Serial.print(", ");
  Serial.println(analogRead(_AIN));

  Serial.print(analogClockSpeed(-1));
  Serial.print(", ");
  Serial.print(getAnalogSampleDuration());
  Serial.print(", ");
  Serial.println(analogRead(_AIN));

  Serial.print(analogClockSpeed(2000));
  Serial.print(", ");

  analogSampleDuration(0);
  Serial.print(getAnalogSampleDuration());
  Serial.print(", ");

  Serial.println(analogRead(_AIN));

}

void loop() {
}

