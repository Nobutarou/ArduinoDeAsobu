// pin definition
// GPIOA-0 = D9 
#define _a0 9 
#define _a1 10 
#define _a2 11 
#define _a3 12 
#define _a4 13 
#define _a5 A0 
#define _a6 A1 
#define _a7 A2 
#define _b0 A3 
#define _b1 2 
#define _b2 3 
#define _b3 4 
#define _b4 5 
#define _b5 6 
#define _b6 7 
#define _b7 8 

// register
#define _gpioa 0x12
#define _gpiob 0x13

// koki 7bit address
#define _address 0b0100000

#include <Wire.h>

void setup() {
  pinMode(_a0,OUTPUT);
  pinMode(_a1,OUTPUT);
  pinMode(_a2,OUTPUT);
  pinMode(_a3,OUTPUT);
  pinMode(_a4,OUTPUT);
  pinMode(_a5,OUTPUT);
  pinMode(_a6,OUTPUT);
  pinMode(_a7,OUTPUT);
  pinMode(_b0,OUTPUT);
  pinMode(_b1,OUTPUT);
  pinMode(_b2,OUTPUT);
  pinMode(_b3,OUTPUT);
  pinMode(_b4,OUTPUT);
  pinMode(_b5,OUTPUT);
  pinMode(_b6,OUTPUT);
  pinMode(_b7,OUTPUT);
  Wire.begin(_address);                
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
}

void receiveEvent(int howMany) {
  unsigned char reg; // レジスタ
  unsigned char data; // データ

  // 2バイト以外は無視
  if(Wire.available() != 2){
    while(Wire.available() > 0){
      Wire.read();
    }; // 空読みする。必要ないかも
    return;
  }; 

  reg=Wire.read();
  data=Wire.read();

  switch(reg){
    case _gpioa:
      digitalWrite(_a0, data & 1); 
      digitalWrite(_a1, (data >> 1) & 1); 
      digitalWrite(_a2, (data >> 2) & 1); 
      digitalWrite(_a3, (data >> 3) & 1); 
      digitalWrite(_a4, (data >> 4) & 1); 
      digitalWrite(_a5, (data >> 5) & 1); 
      digitalWrite(_a6, (data >> 6) & 1); 
      digitalWrite(_a7, (data >> 7) & 1); 
      break;
    case _gpiob:
      digitalWrite(_b0, data & 1); 
      digitalWrite(_b1, (data >> 1) & 1); 
      digitalWrite(_b2, (data >> 2) & 1); 
      digitalWrite(_b3, (data >> 3) & 1); 
      digitalWrite(_b4, (data >> 4) & 1); 
      digitalWrite(_b5, (data >> 5) & 1); 
      digitalWrite(_b6, (data >> 6) & 1); 
      digitalWrite(_b7, (data >> 7) & 1); 
      break;
    default:
      break;
  };
  return;
}
