int a4, a5;
const int a4min=31;
const int a4max=242;
const int a5min=35;
const int a5max=255;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  a4= analogRead(A4);
  a5= analogRead(A5);
  Serial.print("A4: ");
  Serial.print(map(a4, a4min, a4max, 0, 1023) );
  Serial.print(", A5: ");
  Serial.println(map(a5, a5min, a5max, 0, 1023) );
  delay(1000);
}
