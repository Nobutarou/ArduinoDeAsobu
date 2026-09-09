#define cbi(addr, bit) addr &= ~(1 << bit)  // addrのbit目を'0'にする
#define sbi(addr, bit) addr |= (1 << bit)   // addrのbit目を'1'にする
#define _PULSE_PIN 2

double T_end;

ISR(ANALOG_COMP_vect) {
  T_end = micros();
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2 ^ 32 - 1);  //no timeout
  pinMode(_PULSE_PIN, OUTPUT);
  digitalWrite(_PULSE_PIN, LOW);

  // |-|ACME -> 0|-|-|-|ADTS2|ADTS1|ADTS0|
  // ACME: PD6 の代わりに ADC を負入力に使う ->0
  // ADTS: は 22.3.1 に記載なく不明。どうやら割込み処理で ADC を動作させるもののようだ -> 0　
  ADCSRB = 0;

  // |ACD|ACBG|ACO|ACI|ACIE|ACIC|ACIS1|ACIS0|
  // ACD: コンパレータ無効 -> 0
  // ACBG: 正の入力に Vref を使う -> 0
  // ACO: AIN0>AIN1 のときにマイコンが 1 を書き込む
  // ACI: 割り込むときに、マイコンが 1 を書き込む
  // ACIE: 割込み有孔 -> 1
  // ACIC: TIMER1 をうんぬんかんぬんした割込み用。とりあえず理解できないので -> 0
  // A0IS: rising で割り込む --> 11
  ACSR = 0b00001011;

  // |-|-|-|-|-|-|AIN1D|AIN0D|
  // AINxD: デジタル入力無効 -> 1
  DIDR1 = 0b00000011;
}


void discharge() {
  digitalWrite(_PULSE_PIN, LOW);
  delay(1000);
}

unsigned long charge() {
  sbi(PORTD, PORTD2);
  return micros();
}

void loop() {
  double _R;
  _R = r_setup();

  // TinkerCad では文字を送る必要があったが、実物では Enter で反応する
  Serial.println("Enter to reset registor");

  // TinkerCad では不要だったけど、これがないと Serial.available()>0 になってしまう。
  Serial.end();
  Serial.begin(9600);

  while (Serial.available() == 0) {
    discharge();
    cbi(ACSR, ACIE);     // 念のため 一旦割込み無効
    cbi(PORTD, PORTD6);  //念のためグランドに落とす
    cbi(PORTD, PORTD7);  //念のためグランドに落とす
    delay(10);           //念のため delay
    sbi(ACSR, ACIE);     // もう一度割込み有孔
    unsigned long time_start = charge();

    // 遅い while の間に割り込みが発生する手筈
    // 本当は割り込んだら break とかしたいけど、分からない
    while ((ACSR & 0b00100000) != 0b00100000) {};

    double T = T_end - time_start;  // T: 時定数
    // c=-T/R/ln(1-Vc/Vtotal)
    double c = -T / _R / log(0.33333333);
    Serial.println(c, 16);
    digitalWrite(_PULSE_PIN, LOW);
  }
  Serial.end();
  Serial.begin(9600);
}

double r_setup() {
  double registor;
  double x;  // general purpose
  short i;   //
  char a;    // general purpose

  Serial.println("Input the registor [Ohm]");
  Serial.println("Scientific notation only (ex, 1.23e-4)");

  while (Serial.available() == 0) {};
  x = Serial.parseFloat();
  a = Serial.read();
  i = Serial.parseInt();

  registor = x * pow(10.0, i);

  Serial.print("Registor: ");
  Serial.print(registor, 16);
  Serial.println(" Ohm");
  return registor;
}
