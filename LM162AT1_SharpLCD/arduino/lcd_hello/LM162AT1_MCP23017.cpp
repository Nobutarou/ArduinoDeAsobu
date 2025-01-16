// https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C
// を元に LM162AT1 と MCP23017 (PIC のエミュレーション) 向けに書き換えてみた
#include "LM162AT1_MCP23017.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

#define MCP23017_IODIRA  0x00
#define MCP23017_IODIRB  0x01 
#define MCP23017_GPIOA   0x12
#define MCP23017_GPIOB   0x13

#define MCP23017_digitalPinToPort(P)    ((((uint16_t)P) > 0x00ff) ? MCP23017_GPIOB : MCP23017_GPIOA)
#define MCP23017_digitalPinToBitMask(P) ((((uint16_t)P) > 0x00ff) ? (P >> 8) : (P))

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

LiquidCrystal_MCP23017_I2C::LiquidCrystal_MCP23017_I2C(uint8_t i2c_addr) {
    init(false, i2c_addr,
         MCP23017_PA7, MCP23017_PA6, MCP23017_PA5, MCP23017_PA1,
         MCP23017_PB0, MCP23017_PB1, MCP23017_PB2, MCP23017_PB3,
         MCP23017_PB4, MCP23017_PB5, MCP23017_PB6, MCP23017_PB7);
}

LiquidCrystal_MCP23017_I2C::LiquidCrystal_MCP23017_I2C(uint8_t i2c_addr, uint16_t rs, uint16_t rw, uint16_t en, uint16_t backlight,
           uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
           uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
  init(false, i2c_addr, rs, rw, en, backlight, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal_MCP23017_I2C::LiquidCrystal_MCP23017_I2C(uint8_t i2c_addr, uint16_t rs, uint16_t rw, uint16_t en,
			     uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
			     uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
  init(false, i2c_addr, rs, rw, en, 0, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal_MCP23017_I2C::LiquidCrystal_MCP23017_I2C(uint8_t i2c_addr, uint16_t rs, uint16_t rw, uint16_t en, uint16_t backlight,
           uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
  init(true, i2c_addr, rs, rw, en, backlight, 0, 0, 0, 0, d4, d5, d6, d7);
}

LiquidCrystal_MCP23017_I2C::LiquidCrystal_MCP23017_I2C(uint8_t i2c_addr, uint16_t rs, uint16_t rw, uint16_t en,
           uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
  init(true, i2c_addr, rs, rw, en, 0, 0, 0, 0, 0, d4, d5, d6, d7);
}

void LiquidCrystal_MCP23017_I2C::init(bool fourbitmode, uint8_t i2c_addr, uint16_t rs, uint16_t rw, uint16_t en, uint16_t backlight,
			    uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
			    uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
  _i2c_addr = i2c_addr;

  _rs_pin = rs;
  _rw_pin = rw;
  _en_pin = en;
  _backlight_pin = backlight;

  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3;
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7;

  // check if all data pins are on the same port
  // we can do a faster i/o in write4bits and write8bits
  // 4bit モードは firstPin=4
  int firstPin = 0;
  if (fourbitmode) firstPin = 4;

  // DB4~7 を一つのポートにまとめてある場合、データ送信がレジスタ操作で一発なので、ここで
  // チェックしているようだ。
  // まとめていない場合は _data_port=0 となる.
  _data_port = MCP23017_digitalPinToPort(_data_pins[firstPin]);
  for (int i=firstPin+1; i<8; i++) {
	  if (_data_port != MCP23017_digitalPinToPort(_data_pins[i])) {
	    _data_port = 0;
	    break;
	  }
  }

  _gpioa_value = 0x00;
  _gpiob_value = 0x00;

  if (fourbitmode)
    // 0|0|0 = 0
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

//  begin(16, 1);
}

void LiquidCrystal_MCP23017_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    // lines=2
    // LCD_2LINE=0x08=0b1000, 0 |= 0b1000 = 0b1000
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  Wire.begin();

  // set output direction
  // TRISA=0, TRISC=0 に相当
  writeRegister(MCP23017_IODIRA, 0x00);
  writeRegister(MCP23017_IODIRB, 0x00);

  // clear all output pins
  // LATA=0, LATC=0 に相当
  writeRegister(MCP23017_GPIOA, 0x00);
  writeRegister(MCP23017_GPIOB, 0x00);

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  // LM162AT1 は 15ms 待つとある
  delayMicroseconds(20000);

  // Now we pull both RS and R/W low to begin commands
  // LATA=0, LATC=0 やってるから不要な気はする
  writeRS(LOW);
  writeEnable(LOW);
  writeRW(LOW);

  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // こっちが 4bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    // ここ DB7~FB4 = 0b0011 じゃないといけないんだけど、write4bits て引数を 4~7 桁右にシ
    // フトさせてるから 0x03>>4 は 0b11 >> 4 は 0 になっちゃうよね。間違ってない？
    // 元の Adafruit の LiquidCrystal は 0~3 桁のシフトだからこれでいいけど、、、
    //write4bits(0x03);

    // 多分作者さんは 8bit のデバイスみたいだから 4bit モードはなんとなく移植しただけなん
    // だろうな。それでも十分すぎるほどありがたいけど。

    write4bits(0b00110000);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    //write4bits(0x03);
    write4bits(0b00110000);
    //delayMicroseconds(4500); // wait min 4.1ms
    // AT162AT1 は二回目は 100us 以上とある
    delayMicroseconds(150); // wait min 4.1ms

    // third go!
    write4bits(0b00110000);
    //write4bits(0x03);
    // AT162AT1 は待たなくて良いみたい。そもそも write4bits() が呼び出す pulseEnable() に
    // 100us の delay が入っている
    //delayMicroseconds(150);

    // finally, set to 4-bit interface
    //write4bits(0x02);
    // DB7~DB4 に 0b0010 を送るから
    write4bits(0b00100000);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  // _displayfunction = 0b1000 になっている by init()
  // LCD_FUNCTIONSET = 0x20 = 0b100000 なので引数は 0b00101000
  // command() を追うと send(0b00101000, LOW) となる
  // RS=RW=0 とした後 0b00101000 & 0b11110000 = 0b00100000を write4bits() する
  // write4bits() は右に 4桁シフトするから、コマンドは 0b000010 
  // 1桁目は DL=0 は 4ビットモードの選択
  // その後 0b00101000<<4 を write4bits() に送るけど write4bits は >>4 するから結局
  // 右 4ビットの 0b1000 が使用される。4桁目が N=1 は 2桁表示らしい (1/16デューティとある)
  // 3桁目は F=0 で 5x7フォントの指定
  // 要するに DB0~DB7の 8桁の命令を送る関数だったのか。
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  // = 0x04 | 0x00 | 0x00 = 0b100 | 0 | 0 = 0b100
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  
  // この先で LCD_DISPLAYCONTROL | _displaycontrol を command() に送っているのだが
  // 0x08 | 0b100 = 0b1000 | 0b0100 = 0b00001100 なのだがデータシートによると
  // 0b00001000 を送るべきなんだよなあ
  display();
  // データシートを信じて
  //command(0b00001000);

  // 機能がない (ボリュームによる調整)
  //backlight();

  // clear it off
  // LCD_CLEARDISPLAY=1 を command() してるのは OK
  clear();

  // Initialize to default text direction (for romance languages)
  // 0x02 | 0 = 0b10 | 0b00 = 0b10
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  // 0x04 | 0b10 = 0b100 | 0b10 = 0b00000110
  // エントリーモードセットの I/D=1, S=0 
  // 文字を書いたらカーソルは右へ、表示はそのまま。たぶんこれで大丈夫
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_MCP23017_I2C::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystal_MCP23017_I2C::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  //delayMicroseconds(2000);  // this command takes a long time!
  // 最大 1.52ms とのことなので減らす
  delayMicroseconds(1600);  // this command takes a long time!
}

void LiquidCrystal_MCP23017_I2C::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_MCP23017_I2C::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }

  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_MCP23017_I2C::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_MCP23017_I2C::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_MCP23017_I2C::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_MCP23017_I2C::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_MCP23017_I2C::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_MCP23017_I2C::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_MCP23017_I2C::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_MCP23017_I2C::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_MCP23017_I2C::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_MCP23017_I2C::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Turn the (optional) backlight off/on
void LiquidCrystal_MCP23017_I2C::noBacklight(void) {
  if (0 != _backlight_pin) {
    writePin(_backlight_pin, LOW);
  }
}

void LiquidCrystal_MCP23017_I2C::backlight(void) {
  if (0 != _backlight_pin) {
    writePin(_backlight_pin, HIGH);
  }
}

// This will 'right justify' text from the cursor
void LiquidCrystal_MCP23017_I2C::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_MCP23017_I2C::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_MCP23017_I2C::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal_MCP23017_I2C::command(uint8_t value) {
  send(value, LOW);
}

inline size_t LiquidCrystal_MCP23017_I2C::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal_MCP23017_I2C::send(uint8_t value, uint8_t mode) {
  /*
  digitalWrite(_rs_pin, mode);
  */
  writeRS(mode);

  /*
  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) {
    digitalWrite(_rw_pin, LOW);
  }
  */
  writeRW(LOW);

  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value);
  } else {
    write4bits(value & 0xf0);
    write4bits(value << 4);
  }
}

void LiquidCrystal_MCP23017_I2C::pulseEnable(void) {
  /*
  digitalWrite(_enable_pin, LOW);
  */
  writeEnable(LOW);
  delayMicroseconds(1);
  /*
  digitalWrite(_enable_pin, HIGH);
  */
  writeEnable(HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  /*
  digitalWrite(_enable_pin, LOW);
  */
  writeEnable(LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}

void LiquidCrystal_MCP23017_I2C::write4bits(uint8_t value) {
  if (0 == _data_port) {
    for (int i = 4; i < 8; i++) {
      //digitalWrite(_data_pins[i], (value >> i) & 0x01);
      writePin(_data_pins[i], (value >> i) & 0x01);
	}
  }
  else {
	if (MCP23017_GPIOA ==	_data_port) {
	  _gpioa_value &= 0x0f;
	  _gpioa_value |= (value & 0xf0);
	  writeRegister(_data_port, _gpioa_value);
	}
	else {
	  _gpiob_value &= 0x0f;
	  _gpiob_value |= (value & 0xf0);
	  writeRegister(_data_port, _gpiob_value);
	}
  }

  pulseEnable();
}

void LiquidCrystal_MCP23017_I2C::write8bits(uint8_t value) {
  if (0 == _data_port) {
	  for (int i = 0; i < 8; i++) {
		//digitalWrite(_data_pins[i], (value >> i) & 0x01);
		writePin(_data_pins[i], (value >> i) & 0x01);
	  }
  }
  else {
	writeRegister(_data_port, value);
  }

  pulseEnable();
}


/************ low level MCP23017 data pushing commands **************/

void LiquidCrystal_MCP23017_I2C::writeRegister(uint8_t regAddr, uint8_t regValue) {
  Wire.beginTransmission(_i2c_addr);
  Wire.write(regAddr);
  Wire.write(regValue);
  byte error = Wire.endTransmission();
  if (0 != error) {
    if (Serial) {
      Serial.print(F("Wire.write error #")); Serial.println(error);
    }
  }
}

void LiquidCrystal_MCP23017_I2C::writePin(uint16_t pin, uint8_t value) {
  uint8_t regAddr = MCP23017_digitalPinToPort(pin);
  uint8_t bitmask = MCP23017_digitalPinToBitMask(pin);

  if (MCP23017_GPIOA == regAddr) {
    if (value) _gpioa_value |= bitmask;
    else       _gpioa_value &= ~bitmask;
    writeRegister(regAddr, _gpioa_value);
  }
  else {
    if (value) _gpiob_value |= bitmask;
    else       _gpiob_value &= ~bitmask;
    writeRegister(regAddr, _gpiob_value);
  }
}

void LiquidCrystal_MCP23017_I2C::writeRS(uint8_t value) {
  writePin(_rs_pin, value);
}

void LiquidCrystal_MCP23017_I2C::writeRW(uint8_t value) {
  writePin(_rw_pin, value);
}

void LiquidCrystal_MCP23017_I2C::writeEnable(uint8_t value) {
  writePin(_en_pin, value);
}
