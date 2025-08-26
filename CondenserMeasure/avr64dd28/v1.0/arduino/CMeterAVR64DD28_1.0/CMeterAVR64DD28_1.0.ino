#include "LM162AT1_MCP23017.h"

#define LCD_RS_PIN  MCP23017_PB0
#define LCD_RW_PIN  MCP23017_PB5
#define LCD_EN_PIN  MCP23017_PB4
#define LCD_D4_PIN  MCP23017_PB2
#define LCD_D5_PIN  MCP23017_PB3
#define LCD_D6_PIN  MCP23017_PB1
#define LCD_D7_PIN  MCP23017_PA2

#define LCD_I2C_ADDR  0x20

LiquidCrystal_MCP23017_I2C lcd(LCD_I2C_ADDR, LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, 
                               LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

#define _r_s 2.2e3    //R2
#define _r_m  4.7e6 //R1
#define _r_discharge 300.0 //R3

// pin 配置
// R1: PIN_PC0
// R2: PIN_PC1
// R3: PIN_PD2
// AIN: PIN_PD1
// 定義はせずに直接入力することにした。

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
/* {{{ */
  Serial.begin(9600);
  Serial.println("Init");

  lcd.begin(16,2);
  
  // ADC の高速化
  analogClockSpeed(2000); //2MHz
  analogSampleDuration(0); //遅延ゼロ
  analogReadResolution(12); 
  analogRead(PIN_PD1); //空打ち

  //とりあえず 1000ms 放電する
  //1, 充電ピンを input にする （絶縁する)
  //2, 放電ピンを output にする
  //3, 放電ピンを low にする
  // と当初考えたが放電して電流が流れすぎて困るピンもないので、全部 output で全部 low で放
  // 電すればよい。
  
  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);
  pinMode(PIN_PD2, OUTPUT);
  digitalWrite(PIN_PD2, LOW);
  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, LOW);

  // 別のポートにしたら改善するかと思ったけどしない
  //pinMode(PIN_PA6, OUTPUT);
  //digitalWrite(PIN_PA6, LOW);

  // V_Ctest が 1/500 Vcc になるのを待つ
  // 1/500*4095=8
  while ( analogRead(PIN_PD1) > 9){
    delay(1);
  };
/* }}} */
}

void loop() {
  //変数宣言
  unsigned long time_start; //測定開始時刻 [us]
  unsigned long T; //時定数[us]
  double c; //静電容量
  
  //アルゴリズム
  //1, 小R2 で測り、時定数が 1ms を下回ったら R1 へ
  //2, 中R1 で測る

  //1, 小R 開始
  //手順
  //1-1, 充電する
  //1-2, 時定数 T を測る
  //1-3, T>=1000us なら出力して放電して、全部 output low にして終了
  //1-4, 放電、全部 output low して中R へ
  //現時点で、デジタルピンはすべて output の low のはず

  Serial.print("R[ohm]=");
  Serial.println(_r_s,16);
  
  //1-1
  //充電ピンは PIN_PC1
  //PIN_PC0 はお休み
  //充電ピン以外を input にする
  pinMode(PIN_PC0, INPUT);
  pinMode(PIN_PD2, INPUT);
  //pinMode(PIN_PA6, INPUT);

  // これやると 3.3V になる
  // digitalWrite(PIN_PC0,HIGH);
  // while(1);

  //Serial.print("PORTC.DIR=0b");
  //Serial.println(PORTC.DIR,BIN);
  // 0b00000010 となっている。期待通り。

  // 効果なし。
  //analogRead(PIN_PC0);

  //充電ピンを HIGH
  PORTC.OUT = PORTC.OUT | 0b00000010;

  // 効果なし
  //pinMode(PIN_PC0, INPUT);
  time_start=micros();
  
  //1-2
  while ( analogRead(PIN_PD1) < 3684){};//4095*0.9
  T = micros() - time_start; // T: 時定数
  Serial.print("T[us]=");
  Serial.println(T);

  c=(float)T/_r_s; // [uF]
  
  //放電時間 td = -T*ln(Vinf/V0)
  //V0: コンデンサ充電電圧
  //Vinf: 十分に放電した後の電圧
  //V0 = 3.2V 程度のはずだが、安全を取って 5V
  //Vinf: 0.01V とする
  //td = -T*log(1/500)/1000
  //td=-c*_r_discharge*log(1.0/500.0)/1.0e3; //Tが[us]で[ms]欲しいので 1000で割る
  //td=(int)(6.215e-3*c*_r_discharge); 
  //td = (td>1000)*td + (td<=1000)*1000; //気分的に最低でも 1000ms は放電する
  
  //Serial.print("DischargeTime[ms]=");
  //Serial.println(td);

  //と思っていたけど、analogRead() が小さくなるまで待てば良くね？
  //while(1);
  //1-3
  if(T>=1000.0){
    //ポスト処理して完了
    Serial.print("C[uF]=");
    Serial.println(c,16);
    
    lcd.print(c,8);
    lcd.setCursor(0,1);
    lcd.print("uF");

    // discharge
    // R3<R2<R1 なので R2, R1 も一緒に放電しちゃえば良い。
    pinMode(PIN_PC0, OUTPUT);
    pinMode(PIN_PC1, OUTPUT);
    pinMode(PIN_PD2, OUTPUT);
    digitalWrite(PIN_PC0, LOW);
    digitalWrite(PIN_PC1, LOW);
    digitalWrite(PIN_PD2, LOW);

    Serial.end();
  }
  
  //1-4
  // 放電
  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);
  pinMode(PIN_PD2, OUTPUT);
  digitalWrite(PIN_PD2, LOW);
  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, LOW);

  while ( analogRead(PIN_PD1) > 9){
    delay(1);
  };
  
  //2 開始
  //現時点で、デジタルピンはすべて output の low のはず
  Serial.print("R[ohm]=");
  Serial.println(_r_m,16);
  
  //2-1
  //充電ピンは PIN_PC0
  //PIN_PC1 はお休み
  //充電ピン以外を input にする
  pinMode(PIN_PC1, INPUT);
  pinMode(PIN_PD2, INPUT);
  
  //充電ピンを HIGH
  PORTC.OUT = PORTC.OUT | 0b00000001;
  time_start=micros();
  
  //2-2
  while ( analogRead(PIN_PD1) < 3684){};//4095*0.9
  T = micros() - time_start; // T: 時定数
  Serial.print("T[us]=");
  Serial.println(T);

  c=(float)T/_r_m; // [uF]
  
  // 最後は放電しっぱなしなので、td 計算不要。
  //td=(int)(6.215e-3*c*_r_discharge); 
  //td = (td>1000)*td + (td<=1000)*1000; //気分的に最低でも 1000ms は放電する
  //
  //Serial.print("DischargeTime[ms]=");
  //Serial.println(td);
  
  //2-3
  Serial.print("C[uF]=");
  Serial.println(c,16);
  
  lcd.print(c,8);
  lcd.setCursor(0,1);
  lcd.print("uF");

  // discharge
  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);
  pinMode(PIN_PD2, OUTPUT);
  digitalWrite(PIN_PD2, LOW);
  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, LOW);

  Serial.end();
  while(1){};
}  
