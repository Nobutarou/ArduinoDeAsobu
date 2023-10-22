// I apply one Eneloop which has around 1.3V.
// Thus if analog pins work, they should reply around 260.
// Here is the result. 
// A0: 270, A1: 270, A2: 270, A3: 270, A4: 70, A5: 85

// Here is the result when I connect all A-pins to GND.
// A0: 0, A1: 0, A2: 0, A3: 0, A4: 28, A5: 31

// Here is the result when I connect all A-pins to 5V.
// A0: 935, A1: 935, A2: 935, A3: 934, A4: 252, A5: 298

// Here is the result when I connect all A-pins to 3.3V.
// A0: 432, A1: 432, A2: 432, A3: 432, A4: 98, A5: 112

int a0, a1, a2, a3, a4, a5;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  a0= analogRead(A0);
  a1= analogRead(A1);
  a2= analogRead(A2);
  a3= analogRead(A3);
  a4= analogRead(A4);
  a5= analogRead(A5);
  Serial.print("A0: ");
  Serial.print(a0);
  Serial.print(", A1: ");
  Serial.print(a1);
  Serial.print(", A2: ");
  Serial.print(a2);
  Serial.print(", A3: ");
  Serial.print(a3);
  Serial.print(", A4: ");
  Serial.print(a4);
  Serial.print(", A5: ");
  Serial.println(a5);

  delay(1000);
}
