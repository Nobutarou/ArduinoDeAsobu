void setup() {
  Serial.begin(9600);
  Serial.println(analogClockSpeed());
  Serial.println(analogClockSpeed(2000));
}

void loop() {
}
