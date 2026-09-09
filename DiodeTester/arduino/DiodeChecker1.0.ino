#define _digi1 9
#define _digi2 8
#define _ana1 A0
#define _vref 4.8 //手元 Vcc-GND 測定値。動作チェック目的なので適当で良い。

// 配線
// _digi1 - R(1kΩ) - Diode(+-) - _digi2 
// 1. _digi1=high, _digi2=low でダイオードの電圧 Vf を測定
// 2. _digi1=low, _digi2=high でダイオードの電圧 Vr を測定
// 実際には抵抗の電圧を測るので、Vcc から引き算する。
// ダイオードの向きが正しければ Vr > Vf
// 逆になってても回路上問題ないので、単にメッセージで済ます

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  float vf; 
  float vr;

  pinMode(_digi1, OUTPUT);
  pinMode(_digi2, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Enter to start.");
  while (Serial.available() == 0) {}; //打ち込みを待つ
  Serial.read();

  digitalWrite(_digi1, LOW);
  digitalWrite(_digi2, LOW);

  digitalWrite(_digi1, HIGH);
  digitalWrite(_digi2, LOW);
  delay(100);
  vf=_vref*float(analogRead(_ana1))/1023.0;
  delay(100);
  digitalWrite(_digi1, LOW);
  digitalWrite(_digi2, HIGH);
  delay(100);
  vr=_vref*(1.0-float(analogRead(_ana1))/1023.0);

  digitalWrite(_digi1, LOW);
  digitalWrite(_digi2, LOW);

  Serial.print("Vf: ");
  Serial.print(vf,3);
  Serial.println(" V");
  Serial.print("Vr: ");
  Serial.print(vr,3);
  Serial.println(" V");
  if(vf > vr){
    Serial.println("The connection shall be reversed");
  }
  Serial.end();

  resetFunc();
}

void loop() {
  // put your main code here, to run repeatedly:

}
