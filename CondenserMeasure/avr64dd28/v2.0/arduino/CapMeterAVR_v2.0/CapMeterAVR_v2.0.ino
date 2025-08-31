#define DEBUG 1    // デバグ出力不要なら 0
#if DEBUG
#define D_begin(...) Serial.begin(__VA_ARGS__);
#define D_print(...)    Serial.print(__VA_ARGS__)
#define D_write(...)    Serial.write(__VA_ARGS__)
#define D_println(...)  Serial.println(__VA_ARGS__)
#else
#define D_Begin(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

#define DDEBUG 0    // もっと細かいデバグ出力不要なら 0
#if DDEBUG
#define DD_begin(...) Serial.begin(__VA_ARGS__);
#define DD_print(...)    Serial.print(__VA_ARGS__)
#define DD_write(...)    Serial.write(__VA_ARGS__)
#define DD_println(...)  Serial.println(__VA_ARGS__)
#else
#define DD_Begin(...)
#define DD_print(...)
#define DD_write(...)
#define DD_println(...)
#endif

#define C1 4.7e-6
#define C2 470.0e-12

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


// DxCore の fuse6 の MVSYSCFG が間違いで 0b01 = Dual となってしまっているので、手作業で
// 0b10 に書き換えるか、Cポートを避ける必要がある。

void setup(){
  // setup

  D_begin(9600);
  analogReadResolution(12);
  lcd.begin(16, 2);
  word adc; //word = unsigned int = 2^16
  float ctest;

  // 空打ちしておくと 2回目から安定する気がする。
  analogRead(PIN_PC1);
  analogRead(PIN_PC3);
  analogRead(PIN_PD2);

  // 1. PC0, PC2, PD1 を output low で放電。PC1, PC3, PD2 で analogRead で放電完了判定。
  // 2. PC0,PD1 は input で絶縁。PC2 を output high で充電。PC3 で充電完了判定して PD2 で電圧読
  //    み取り
  // 3. 放電
  // 4. PC2, PD1 は input で絶縁、PC0 を output high で充電。PC1,PD2 で判定と測定
  // 5. 放電

  // 1. 放電
  D_println("1, discharging");
  pinMode(PIN_PC0,OUTPUT);
  pinMode(PIN_PC2,OUTPUT);
  pinMode(PIN_PD1,OUTPUT);
  digitalWrite(PIN_PC0,LOW);
  digitalWrite(PIN_PC2,LOW);
  digitalWrite(PIN_PD1,LOW);
  //12bit analogRead の 10 は 0.24% くらい。
  while(analogRead(PIN_PC1)>10){
    delay(100);
  };
  while(analogRead(PIN_PC3)>10){
    delay(100);
  };
  while(analogRead(PIN_PD2)>10){
    delay(100);
  };
  delay(100);
  D_println("1, discharged");

  // 2. PC0,PD1 は input で絶縁。PC2 を output high で充電。PC3 で充電完了判定して PD2 で電圧読
  //    み取り
  D_println("2, charging");
  pinMode(PIN_PC0,INPUT);
  pinMode(PIN_PD1,INPUT);
  //必要ないが速度もプログラムサイズも気にしなくて良いので分かり易さを重視する
  pinMode(PIN_PC2,OUTPUT);
  digitalWrite(PIN_PC2,HIGH);
  while(analogRead(PIN_PC3)<4085){
    delay(100);
  };
  delay(100);
  D_println("2, charged");
  adc=analogRead(PIN_PD2);

  // CT=CB/100 のとき ADC=4054。なので ADC<4055 なら結果表示して終了
  if(adc<4055){
    ctest=4095.0/float(adc)*C1-C1;
    ctest=ctest*1.0e6; // F → uF
    D_print("Ctest[uF]=");
    D_println(ctest,5);
    // 終る前に放電しておく
    D_println("5, discharging");

    lcd.print(ctest,12);
    lcd.setCursor(0,1);
    lcd.print("uF");

    pinMode(PIN_PC0,OUTPUT);
    pinMode(PIN_PC2,OUTPUT);
    pinMode(PIN_PD1,OUTPUT);
    digitalWrite(PIN_PC0,LOW);
    digitalWrite(PIN_PC2,LOW);
    digitalWrite(PIN_PD1,LOW);
    while(1);
  };

  //3. 放電
  D_println("3, discharging");
  pinMode(PIN_PC0,OUTPUT);
  pinMode(PIN_PC2,OUTPUT);
  pinMode(PIN_PD1,OUTPUT);
  digitalWrite(PIN_PC0,LOW);
  digitalWrite(PIN_PC2,LOW);
  digitalWrite(PIN_PD1,LOW);
  while(analogRead(PIN_PC0)>10){
    delay(100);
  };
  while(analogRead(PIN_PC3)>10){
    delay(100);
  };
  while(analogRead(PIN_PD2)>10){
    delay(100);
  };
  delay(100);
  D_println("3, discharged");

  // 4. PC2, PD1 は input で絶縁、PC0 を output high で充電。PC1,PD2 で判定と測定
  D_println("4, charging");
  pinMode(PIN_PC2,INPUT);
  pinMode(PIN_PD1,INPUT);
  pinMode(PIN_PC0,OUTPUT);
  digitalWrite(PIN_PC0,HIGH);
  while(analogRead(PIN_PC1)<4085){
    delay(100);
  };
  delay(100);
  D_println("4, charged");
  adc=analogRead(PIN_PD2);
  DD_print("adc=");
  DD_println(adc);
  ctest=4095.0/float(adc)*C2-C2;
  DD_print("ctest=");
  DD_println(ctest,16);

  // 単位を pF か uF に
  if(ctest<0.01e-6){
    ctest=ctest*1.0e12; //pF
    D_print("Ctest[pF]=");
    D_println(ctest,5);

    lcd.print(ctest,12);
    lcd.setCursor(0,1);
    lcd.print("pF");
  } else {
    ctest=ctest*1.0e6; // uF
    D_print("Ctest[uF]=");
    D_println(ctest,5);

    lcd.print(ctest,12);
    lcd.setCursor(0,1);
    lcd.print("uF");
  };
  // 5. 終る前に放電しておく
  D_println("5, discharging");
  pinMode(PIN_PC0,OUTPUT);
  pinMode(PIN_PC2,OUTPUT);
  pinMode(PIN_PD1,OUTPUT);
  digitalWrite(PIN_PC0,LOW);
  digitalWrite(PIN_PC2,LOW);
  digitalWrite(PIN_PD1,LOW);
  while(1);
  
}

void loop(){
}

