// PORTC の analogRead() が変だから、自力でやってみる。
// PC0 --> AIN28

// うん測れていない。

void setup()
{
  VREF.ADC0REF=0b10000101; // VREF is always on. VREF=VDD. 16進表記やめろ
  //VREF.ADC0REF=0; // VREF is always on. VREF=internal 1.024V. 16進表記やめろ
  ADC0.CTRLC=0xB; // prescaler = 96
  ADC0.MUXPOS=28; // AIN28。なんで 16進数で書くかな
  //ADC0.MUXPOS=29; //PC1
  ADC0.CTRLA=0b00000001; // Enable = 1. 解像度はデフォルトで 12bit

  ADC0.COMMAND=1; //start。終ると 0 になる
  while( ( ADC0.COMMAND) & 1 ){delay(1);};

  Serial.begin(9600);
  Serial.print("ADC.RES=");
  Serial.println(ADC0.RES);
  Serial.print("ADC.RESH=");
  Serial.println(ADC0.RESH,BIN);
  Serial.print("ADC.RESL=");
  Serial.println(ADC0.RESL,BIN);
  Serial.print("Result=");
  Serial.println( (ADC0.RESH<<8) + ADC0.RESL);

}

void loop()
{
}
