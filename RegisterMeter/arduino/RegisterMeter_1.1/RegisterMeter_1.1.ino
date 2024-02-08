#define cbi(addr, bit) addr &= ~(1 << bit)  // addrのbit目を'0'にする
#define sbi(addr, bit) addr |= (1 << bit)   // addrのbit目を'1'にする
#define rbi(addr,bit) addr ^= (1 << bit) //addr のbit を反転
#define chkbi(addr,bit) ((addr >> bit) & 1)  //addr の bit の読み取り

//応答速度はどうでもよいので Arduino のコマンドだけでやれる
//デジタルピンと抵抗の対応
#define _pin_R0_1 2  //300Ω PB0
#define _pin_R0_2 3  //22kΩ PD5
#define _pin_R0_3 4  //1MΩ PD6

#define _R0_1 300.0
#define _R0_2 22.0e3 
#define _R0_3 1.0e6 

#define _analog_pin_V1 A5 // 対象抵抗電圧測定用
#define _analog_pin_V0_1 A4 // 小抵抗出力ピン電圧測定用
#define _analog_pin_V0_2 A3 // 中抵抗出力ピン電圧測定用
#define _analog_pin_V0_3 A2 // 高抵抗出力ピン電圧測定用

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void digital_low()
{
  //すべて output の low に。
  //ゆっくり Arduino コマンドで OK
  pinMode(_pin_R0_1,OUTPUT);
  pinMode(_pin_R0_2,OUTPUT);
  pinMode(_pin_R0_3,OUTPUT);
  
  digitalWrite(_pin_R0_1, LOW);
  digitalWrite(_pin_R0_2, LOW);
  digitalWrite(_pin_R0_3, LOW);
}

void charge(int pin_charge) {
  //手順
  //1, 充電ピン以外を input にする
  //2, 充電ピンを high にする

  switch(pin_charge){
    case  _pin_R0_1:
      //1, ゆっくり
      pinMode(_pin_R0_1, OUTPUT);
      pinMode(_pin_R0_2, INPUT);
      pinMode(_pin_R0_3, INPUT);
      
      //2, 
      digitalWrite(_pin_R0_1, HIGH);
      break;

    case  _pin_R0_2:
      //1,
      pinMode(_pin_R0_1, INPUT);
      pinMode(_pin_R0_2, OUTPUT);
      pinMode(_pin_R0_3, INPUT);
      
      //2, 
      digitalWrite(_pin_R0_2, HIGH);
      break;

    case  _pin_R0_3:
      //1, ゆっくり
      pinMode(_pin_R0_1, INPUT);
      pinMode(_pin_R0_2, INPUT);
      pinMode(_pin_R0_3, OUTPUT);
      
      //2, 
      digitalWrite(_pin_R0_3, HIGH);
      break;

  }
  return;
}

void show_result(double R1, String unit){
  Serial.print("R: ");
  Serial.print(R1, 8);
  Serial.println(unit);
  return;
}

void loop_again(){
  Serial.println("Enter (or reset button) to restart measureing.");
  while (Serial.available() == 0) {}; //打ち込みを待つ
  Serial.read();
  Serial.println("Restarting...");
  resetFunc();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");
  
  //とりあえずデジタルピンはすべて low
  digital_low();
  delay(1000);

  Serial.println("Enter to start measureing.");
  while (Serial.available() == 0) {}; //打ち込みを待つ
  Serial.read();
}

void loop() {
  //変数宣言
  double R0; //充電用抵抗
  double R1; //測定抵抗
  int pin_charge; //電源ピン、まあチャージでいいか。
  int a0; //対象抵抗アナログ入力値
  int a1; //出力アナログ入力値
  String unit; //単位。serial print 用
  
  //アルゴリズム
  //1, 200Ωで測定、抵抗電圧が出力電圧の 90% を超えたら次の抵抗へ
  //2, 300Ω、以下同文
  //3, 22kΩ、以下同文
  //4, 10MΩ、以下同文
  
    //1, 200Ω 開始
  //手順
  //1-1, 200Ωだけ output の high, 他は input
  //1-2, アナログ値読む
  //1-3, 1-2 が 923 超えなければ R 計算して終了
  //1-4, そうでなければ次へ
  
  //現時点で、デジタルピンはすべて output の low のはず
  Serial.print("Measuring with ");
  Serial.print(_R0_1,16);
  Serial.println(" ohm register");

  R0 = _R0_1;
  
  //1-1
  pin_charge = _pin_R0_1;
  charge(pin_charge);
  delay(1000);
  
  //1-2
  a0=analogRead(_analog_pin_V0_1);
  Serial.print("V0: Analog read: ");
  Serial.println(a0);

  a1=analogRead(_analog_pin_V1);
  Serial.print("V1: Analog read: ");
  Serial.println(a1);

  //1-3
  if( (double) a1/a0 <= 0.9){
    // V1/V0 = R1/(R0+R1)
    // ... R1 = R0*V1 / (V0-V1)
    // a = V1/VCC * V0
    // ... V1 = a * VCC /V0
    // ... R1 = R0*a1/(a0-a1)
    
    R1= R0*a1/(a0-a1);
    
    switch(R1>1000.0){
      case 0:
        unit=" ohm";
        break;
      case 1:
        unit=" kohm";
        R1=R1*1.0e-3;
        break;
    }
    show_result(R1, unit);      
    
    loop_again();
  }
  
  //1-4
  digital_low();
  delay(1000);

  Serial.print("Measuring with ");
  Serial.print(_R0_2,16);
  Serial.println(" ohm register");

  R0 = _R0_2;
  
  //2-1
  pin_charge = _pin_R0_2;
  charge(pin_charge);
  delay(1000);
  
  //2-2
  a0=analogRead(_analog_pin_V0_2);
  Serial.print("V0: Analog read: ");
  Serial.println(a0);

  a1=analogRead(_analog_pin_V1);
  Serial.print("V1: Analog read: ");
  Serial.println(a1);

  //2-3
  if( (double) a1/a0 <= 0.9){
    // V1/V0 = R1/(R0+R1)
    // ... R1 = R0*V1 / (V0-V1)
    // a = V1/VCC * V0
    // ... V1 = a * VCC /V0
    // ... R1 = R0*a1/(a0-a1)
    
    R1= R0*a1/(a0-a1);
    
    unit=" kohm";
    R1=R1*1.0e-3;

    show_result(R1, unit);      
    
    loop_again();
  }
  
  //2-4
  digital_low();
  delay(1000);

  Serial.print("Measuring with ");
  Serial.print(_R0_3,16);
  Serial.println(" ohm register");

  R0 = _R0_3;
  
  //3-1
  pin_charge = _pin_R0_3;
  charge(pin_charge);
  delay(1000);
  
  //3-2
  //a1 を先に取らないと値がおかしい
  a1=analogRead(_analog_pin_V1);
  Serial.print("V1: Analog read: ");
  Serial.println(a1);

  //1023にへばりついてるから、もう測定しない
  a0=1023;
  Serial.print("V0: Analog is decided to: ");
  Serial.println(a0);

  //3-3
  R1= R0*a1/(a0-a1);

  //3-3
  switch(R1>1.0e6){
    case 0:
      unit=" kohm";
      R1=R1*1.0e-3;
      break;
    case 1:
      unit=" Mohm";
      R1=R1*1.0e-6;
      break;
  }
  show_result(R1, unit);      
    
  loop_again();

}  
