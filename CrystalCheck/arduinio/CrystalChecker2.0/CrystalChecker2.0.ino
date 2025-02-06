// ASOBoard D2 にカウンタの QH を繋ぐ
// 1s の間 FALLING をカウントする 

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

unsigned long count = 0;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  unsigned long final_count; 
  float mhz;
  pinMode(2,INPUT);
  delay(100);
  attachInterrupt(digitalPinToInterrupt(2),falling,FALLING);
  delay(1000);
  final_count = count; //もたついてる間に count が進むのが嫌だ
  detachInterrupt(digitalPinToInterrupt(2)); //念の為止めておく

  mhz=final_count*256.0/1.0e6;

  Serial.begin(9600);
  Serial.print("Count: ");
  Serial.println(final_count);
  Serial.print("Freq [MHz]: ");
  Serial.println(mhz);
  Serial.end();

  lcd.begin(16, 2);
  lcd.print(mhz,12);
  lcd.setCursor(0,1);
  lcd.print("[MHz]");
  while(1);
}

void loop()
{
}

void falling()
{
  count++;
}
