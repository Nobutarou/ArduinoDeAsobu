#include "LM162AT1_MCP23017.h"

#define LCD_RS_PIN  MCP23017_PB0
#define LCD_RW_PIN  MCP23017_PB5
#define LCD_EN_PIN  MCP23017_PB4

//#define LCD_D0_PIN  MCP23017_PB0
//#define LCD_D1_PIN  MCP23017_PB1
//#define LCD_D2_PIN  MCP23017_PB2
//#define LCD_D3_PIN  MCP23017_PB3
#define LCD_D4_PIN  MCP23017_PB2
#define LCD_D5_PIN  MCP23017_PB3
#define LCD_D6_PIN  MCP23017_PB1
#define LCD_D7_PIN  MCP23017_PA2

//#define LCD_BACKLIGHT_PIN MCP23017_PA1

#define LCD_I2C_ADDR  0x20

// R/W を GND に落とすパターンはライブラリからは削られている。

LiquidCrystal_MCP23017_I2C lcd(LCD_I2C_ADDR, LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, 
                               LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
//  delay(100);
  lcd.begin(16, 2);
  lcd.print("Hello World!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
