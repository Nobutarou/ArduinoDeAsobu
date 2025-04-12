#define _digi1 3
#define _digi2 4
#define _ana1 A0
#define _vref 5.0

// LCD
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
//  Serial.println("Enter to start.");
//  while (Serial.available() == 0) {}; //打ち込みを待つ
//  Serial.read();

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

  lcd.begin(16, 2);
  lcd.print("Vf: ");
  lcd.print(vf,3);
  lcd.print(" V");
  lcd.setCursor(0,1);
  lcd.print("Vr: ");
  lcd.print(vr,3);
  lcd.print(" V");

  if(vf > vr){
    Serial.println("The connection shall be reversed");
    lcd.print("***");
  }
  Serial.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
