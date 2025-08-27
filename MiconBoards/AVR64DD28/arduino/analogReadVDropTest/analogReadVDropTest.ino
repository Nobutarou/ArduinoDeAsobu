// PC0 HIGH - R + PC1 digitai In
//              + PC2 analogRead


void setup() {
  Serial.begin(9600);

  analogReadResolution(12);
  analogSampleDuration(0);
  analogRead(PIN_PD1);

  pinMode(PIN_PC0,OUTPUT);
  pinMode(PIN_PC1,INPUT);
  digitalWrite(PIN_PC0,HIGH);
}

void loop() {
  unsigned short i;  
  unsigned short a;  

  Serial.print("ADCx1=");
  Serial.print(analogRead(PIN_PD1));

  while(i<10000){
    a=analogRead(PIN_PD1);
    i++;
  }
  Serial.print(", ADCx10000=");
  Serial.print(a);
  Serial.print(", PC1=");
  Serial.println(digitalRead(PIN_PC1));

  // 結果
  // | C0-C1 抵抗[Ω] | ADC 1回 | ADC 10000回 | 
  // | ---            | ---     | ---         |
  // | 10M            | 4095    | 2065        |
  // | 1M             | 4095    | 3172        |
  // | 100k           | 4095    | 3965        |
  // |  10k           | 4095    | 4095        |
}

