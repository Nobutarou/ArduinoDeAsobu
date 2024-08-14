#define cbi(addr, bit) addr &= ~(1 << bit)  // addrのbit目を'0'にする
#define sbi(addr, bit) addr |= (1 << bit)   // addrのbit目を'1'にする
#define rbi(addr,bit) addr ^= (1 << bit) //addr のbit を反転
#define chkbi(addr,bit) ((addr >> bit) & 1)  //addr の bit の読み取り

#define _vin 1.1

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  unsigned int adc;
  float vcc;
  // ADMUX = REFS1, REFS0, ADLAR, -, MUX3, MUX2, MUX1, MUX0
  // REFS1,0 = 01 : Vref=AVcc(=Vcc in my case)
  // ADLAR = 0: 結果は右詰め
  // MUX3,,0 = 1110 : Input is 1.1V
  ADMUX = 0b01001110;
  delay(10); // Vref の安定を待つ

  // ADCSRA = ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2, ADPS1, ADPS0
  // 基本的な設定は Arduino がやってるので ADSC=1 で ADC 開始するだけ
  sbi(ADCSRA,ADSC); 

  while(chkbi(ADCSRA,ADSC)); // ADSC=0, ADC 完了まで待つ
  adc = ADCL + (ADCH << 8);
  vcc = _vin * 1023.0 / float(adc);
  Serial.begin(9600);
  Serial.print("Vcc: ");
  Serial.print(vcc, 3);
  Serial.println("[V]");
  Serial.println("Enter to Re-measure.");
  while (Serial.available() == 0) {};
  Serial.read();
  Serial.end();
  resetFunc();
}

void loop() {
  // put your main code here, to run repeatedly:

}
