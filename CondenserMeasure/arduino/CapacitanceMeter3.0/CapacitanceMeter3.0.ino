#define cbi(addr, bit) addr &= ~(1 << bit)  // addrのbit目を'0'にする
#define sbi(addr, bit) addr |= (1 << bit)   // addrのbit目を'1'にする
#define rbi(addr,bit) addr ^= (1 << bit) //addr のbit を反転
#define chkbi(addr,bit) ((addr >> bit) & 1)  //addr の bit の読み取り

//充電ピンは、1系統の PORTx にまとめること
#define _pin_charge_s_r 5  //小さい抵抗用, PD5
#define _pin_charge_m_r  6  //中くらいの抵抗用, PD6
#define _pin_charge_l_r 7  //大きい抵抗用, PD7

#define _pin_charge_port PORTD //充電ピン用ポート
#define _pin_charge_s_reg PORTD5 //小抵抗ポートレジスタ
#define _pin_charge_m_reg PORTD6 //中抵抗ポートレジスタ
#define _pin_charge_l_reg PORTD7 //大抵抗ポートレジスタ

#define _pin_discharge 8 //放電用, PB0

#define _r_s 1.0e3    //小さい抵抗[Ω]
#define _r_m  100.0e3 //中くらい抵抗[Ω]
#define _r_l 10.0e6 //大抵抗[Ω] 300Ωの付け替えをしたときに壊れたのか？10MΩなんだが。。。
#define _r_discharge 300.0 //放電抵抗[Ω]

#define _analog_pin A0 //測定用アナログピン, PC0

// とりあえず決めておいて、設計の都合で変更できるように

#define _lcd_rs 12
#define _lcd_en 13
#define _lcd_d4 10
#define _lcd_d5 9
#define _lcd_d6 A1
#define _lcd_d7 11

#include <LiquidCrystal.h>

const int rs=_lcd_rs, en=_lcd_en, d4=_lcd_d4, d5=_lcd_d5, d6=_lcd_d6, d7=_lcd_d7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  lcd.begin(16,2);
  
  // ADC の高速化
  // ADCSRA の ADPS0=ADPS2=0, ADPS1=1 とする
  cbi(ADCSRA,ADPS0);
  cbi(ADCSRA,ADPS2);
  sbi(ADCSRA,ADPS1);

  //まず 1000ms 放電する
  discharge();
  delay(1000);
  
  //デジタル4ピンを全部 low にする
  digital_low();
  delay(100);
}

void loop() {
  //変数宣言
  double R; //充電用抵抗
  unsigned long time_start; //測定開始時刻 [us]
  double T; //時定数[us]
  unsigned int td; //放電時間[ms]
  double c; //静電容量
  int pin_charge; //充電ピン
  double R_discharge= _r_discharge;
  
  //アルゴリズム
  //1, 小R で測り、時定数が 1e3us 超えたら完了
  //2, 中R で測り、時定数が 1e3us 超えたら完了
  //3, 大R で測る。時定数が何であれ完了

  //1, 小R 開始
  //手順
  //1-1, 充電する
  //1-2, 時定数 T を測る
  //1-3, T>=1000us なら出力して放電して、全部 output low にして終了
  //1-4, 放電、全部 output low して中R へ
  //現時点で、デジタルピンはすべて output の low のはず
  Serial.print("Measuring with ");
  Serial.print(_r_s,c);
  Serial.println(" ohm register");
  
  R = _r_s;
  
  //1-1
  pin_charge = _pin_charge_s_r;
  time_start = charge(pin_charge);
  //この時点で、充電ピンは output の high, 他3ピンは input
  
  //1-2
  while ( analogRead(_analog_pin) < 647){};//1023*64.2=646.5。
  T = micros() - time_start; // T: 時定数
  Serial.print("Tau: ");
  Serial.print(T,16);
  Serial.println(" [us]");

  c=T/R; // [uF]

  
  //放電時間 td = -T*ln(Vinf/V0)
  //V0: コンデンサ充電電圧
  //Vinf: 十分に放電した後の電圧
  //V0 = 3.2V 程度のはずだが、安全を取って 5V
  //Vinf: 0.01V とする
  //td = -T*log(1/500)/1000
  td=-c*R_discharge*log(1.0/500.0)/1.0e3; //Tが[us]で[ms]欲しいので 1000で割る
  td = (td>1000)*td + (td<=1000)*1000; //気分的に最低でも 1000ms は放電する
  
  Serial.print("time to discharge: ");
  Serial.print(td);
  Serial.println(" [ms]"); 
  
  //1-3
  if(T>=1000.0){
    Serial.print("Capacitance: ");
    Serial.print(c,16);
    Serial.println(" [uF]");
    
    lcd.print(c,14);
    lcd.setCursor(0,1);
    lcd.print("uF");

    discharge();
    delay(td);
    digital_low();
    delay(100);

    Serial.end();
    while(1){};
  }
  
  //1-4
  discharge();
  delay(td);
  digital_low();
  delay(100);
  
  //2 開始
  //現時点で、デジタルピンはすべて output の low のはず
  Serial.print("Measuring with ");
  Serial.print(_r_m,c);
  Serial.println(" ohm register");
  R = _r_m;
  
  //2-1
  pin_charge = _pin_charge_m_r;
  time_start = charge(pin_charge);
  //この時点で、充電ピンは output の high, 他3ピンは input
  
  //2-2
  while ( analogRead(_analog_pin) < 647){};//1023*64.2=646.5。
  T = micros() - time_start; // T: 時定数
  Serial.print("Tau: ");
  Serial.print(T,16);
  Serial.println(" [us]");

  td=-c*R_discharge*log(1.0/500.0)/1.0e3; //Tが[us]で[ms]欲しいので 1000で割る
  td = (td>1000)*td + (td<=1000)*1000;

  Serial.print("time to discharge: ");
  Serial.print(td);
  Serial.println(" [ms]"); 

  
  //2-3
  if(T>=1000.0){
    c=T/R; // [uF]
    Serial.print("Capacitance: ");
    Serial.print(c,16);
    Serial.println(" [uF]");

    lcd.print(c,14);
    lcd.setCursor(0,1);
    lcd.print("uF");
    
    discharge();
    delay(td);
    digital_low();
    delay(100);

    Serial.end();
    while(1){};
  }
  
  //2-4
  discharge();
  delay(td);
  digital_low();
  delay(100);
  
  //3 開始
  //現時点で、デジタルピンはすべて output の low のはず
  Serial.print("Measuring with ");
  Serial.print(_r_l,c);
  Serial.println(" ohm register");

  R = _r_l;
  
  //3-1
  pin_charge = _pin_charge_l_r;
  time_start = charge(pin_charge);
  //この時点で、充電ピンは output の high, 他3ピンは input
  
  //3-2
  while ( analogRead(_analog_pin) < 647){};//1023*64.2=646.5。
  T = micros() - time_start; // T: 時定数
  Serial.print("Tau: ");
  Serial.print(T,16);
  Serial.println(" [us]");

  td=-c*R_discharge*log(1.0/500.0)/1.0e3; //Tが[us]で[ms]欲しいので 1000で割る
  td = (td>1000)*td + (td<=1000)*1000;

  Serial.print("time to discharge: ");
  Serial.print(td);
  Serial.println(" [ms]"); 
  
  
  c=T/R; // [uF]
  Serial.print("Capacitance: ");
  if(c>=0.005){
    Serial.print(c,16);
    Serial.println(" [uF]");
    lcd.print(c,14);
    lcd.setCursor(0,1);
    lcd.print("uF");
    
  } else {
    Serial.print(c*1.0e6,16);
    Serial.println(" [pF]");
    lcd.print(c*1.0e6,14);
    lcd.setCursor(0,1);
    lcd.print("uF");
    
  }
    
  discharge();
  delay(td);
  digital_low();
  delay(100);

  Serial.end();
  while(1){};

}  

unsigned long charge(int pin_charge) {
    //手順
  //1, 充電ピン以外を input にする
  //2, 充電ピンを high にする
  //3, 時刻を返す

  switch(pin_charge){
    case  _pin_charge_s_r:
      //1, ゆっくり
      pinMode(_pin_charge_m_r, INPUT);
      pinMode(_pin_charge_l_r, INPUT);
      pinMode(_pin_discharge, INPUT);
      //2, 速度が重要なのでレジスタ操作
      sbi(_pin_charge_port,_pin_charge_s_reg);
      //3
      return micros();
      break;
      //3 をケースの外に出したいけど、遅れがあると嫌なので
    case  _pin_charge_m_r:
      pinMode(_pin_charge_s_r, INPUT);
      pinMode(_pin_charge_l_r, INPUT);
      pinMode(_pin_discharge, INPUT);
      sbi(_pin_charge_port,_pin_charge_m_reg);
      return micros();
      break;
    case  _pin_charge_l_r:
      pinMode(_pin_charge_s_r, INPUT);
      pinMode(_pin_charge_m_r, INPUT);
      pinMode(_pin_discharge, INPUT);
      sbi(_pin_charge_port,_pin_charge_l_reg);
      return micros();
      break;    
  }
}


void digital_low()
{
  //ゆっくり
  pinMode(_pin_charge_s_r, OUTPUT);
  pinMode(_pin_charge_m_r, OUTPUT);
  pinMode(_pin_charge_l_r, OUTPUT);
  pinMode(_pin_discharge, OUTPUT);
  
  digitalWrite(_pin_charge_s_r, LOW);
  digitalWrite(_pin_charge_m_r, LOW);
  digitalWrite(_pin_charge_l_r, LOW);
  digitalWrite(_pin_discharge, LOW);
}
  

void discharge() {
  //手順
  //1, 充電ピンを input にする （絶縁する)
  //2, 放電ピンを output にする
  //3, 放電ピンを low にする
  
  //1, ゆっくりで構わないので、Arduino 関数を使う
  pinMode(_pin_charge_s_r, INPUT);
  pinMode(_pin_charge_m_r, INPUT);
  pinMode(_pin_charge_l_r, INPUT);
  
  //2, やはりゆっくりで良いので Arduino 関数
  pinMode(_pin_discharge, OUTPUT);
  
  //3, のんびりと
  digitalWrite(_pin_discharge, LOW);
}

