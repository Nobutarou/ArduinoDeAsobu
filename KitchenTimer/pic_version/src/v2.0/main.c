/*
 * File:   main.c
 * Author: snob
 * ver 2.0
 */

// CONFIG1
/*{{{*/
// clock
// 5.2.2 に内蔵クロックなら FOSC = INTOSC
// あとは main() 内で
// 5.2.2.1 で OSCTUNEbits.TUN で調整しろ --> 工場校正済の 0b000000
// OSCCONbits.IRCF で周波数選べ --> 1MHz にしてみよう。
#pragma config FOSC = INTOSC

// Watch dog 使える実力ないから要らない
#pragma config WDTE = OFF

// 電源安定化のために少しゆっくり起動してくれるんでしょ。なんで初期値無効なんだろ
#pragma config PWRTE = ON

// 書き換えのために要るよね？
#pragma config MCLRE = ON

// 保護なんてしたら間違ったプログラムを直せなくならない？
#pragma config CP = OFF
#pragma config CPD = OFF

// 良く分かんないけど壊れる前に止まってくれるほうが良い
#pragma config BOREN = ON       

// 周波数の出力がされても使い道分からない。
#pragma config CLKOUTEN = OFF

// 内蔵クロックしか使わない
// 省電力のための切り替えも Failsafe で内蔵に切り替わるのも要らない
#pragma config IESO = OFF 
#pragma config FCMEN = OFF

// CONFIG2
// 保護なんてしたら、直せなくならない?
#pragma config WRT = OFF       

// 16MHz で満足です
#pragma config PLLEN = OFF

// stack over flow で落ちる。まあ動作を続けさす理由もない
#pragma config STVREN = ON 

// 2.45V くらいの電圧低下で落ちる
#pragma config BORV = LO 

// デバッガなんて高くて買えないさ
#pragma config DEBUG = OFF

// 一度 LVP=on にすると高電圧モードでしか off にできないらしいが、off にできなくて問題なく
// なくない? 今の計画では低電圧モードでの書き込みしかできないし
#pragma config LVP = ON

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/*}}}*/

#include <xc.h>
#define _XTAL_FREQ 1000000 
#define _C    262 // [Hz]
#define _D    294
#define _E    330
#define _F    349
#define _G    392
#define _A    440
#define _B    494
#define _CH   523
#define _haku 250000 // [us]

void melody(void);
void tone(int n_haku, int freq);
void noTone(int n_haku);

void main(void) {
  // 16MHz clock にする設定
  OSCTUNEbits.TUN = 0b000000; // 多分要らない
  OSCCONbits.IRCF = 0b1011; // 1MHz

  PORTA = 0;
  // Analog input は使わない
  ANSELA = 0;
  // RA1,2,4,5 は input (=1), RA0 は output (=0)
  // RA3 を GND に落としたままだと起動できないらしい。
  
  TRISA = 0b00111100;
  __delay_ms(1000);

  // RA1 が 1桁目で RA5 が 4桁目とする
  // __delay_ms の最大値は 16MHz で 12000ms (=12s) くらい
  // 1MHz なら x16 = 192000 (192s) くらいのはず
  if(PORTAbits.RA1 == 1){ 
    __delay_ms(60000);
  }
  if(PORTAbits.RA2 == 1){ 
    __delay_ms(120000);
  }
  if(PORTAbits.RA4 == 1){ 
    __delay_ms(120000);
    __delay_ms(120000);
  }
  if(PORTAbits.RA5 == 1){
    __delay_ms(120000);
    __delay_ms(120000);
    __delay_ms(120000);
    __delay_ms(120000);
  }

  melody();
  while(1){};
  return;
}

void melody(void)
{
  tone(2, _C);
  tone(2, _D);
  tone(2, _E);
  tone(2, _F);

  tone(4, _G);
  tone(4, _CH);

  tone(4, _A);
  tone(2, _G);
  tone(2, _F);

  tone(4, _G);
  noTone(4);

  tone(4, _F);
  tone(2, _E);
  tone(2, _D);

  tone(4, _E);
  tone(4, _C);

  tone(2, _D);
  tone(2, _D);
  tone(2, _D);
  tone(2, _E);

  tone(4, _D);
  noTone(4);

  tone(2, _C);
  tone(2, _D);
  tone(2, _E);
  tone(2, _F);

  tone(4, _G);
  tone(4, _CH);

  tone(4, _A);
  tone(2, _G);
  tone(2, _F);

  tone(4, _G);
  noTone(4);

  tone(4, _F);
  tone(2, _E);
  tone(2, _D);

  tone(4, _E);
  tone(4, _C);

  tone(2, _D);
  tone(2, _D);
  tone(2, _D);
  tone(2, _E);

  tone(4, _C);
  noTone(4);
}

//void tone(int n_haku, int freq)
//{
//  // PWN でサイン波とかに挑戦せずに、Hi, low を繰り返すだけにする。
//
//  // 音を出す全部の長さは t_all = n_haku*_haku [us]
//  // High にする時間は dt_h = 1/freq/2 [s] = 1e6/freq/2[us]
//  // Low の時間も同じ  dt_l = dt_h
//  // High, Low 合わせて dt = dt_h + dt_l = 1e6/freq [us]
//  // High, Low 出す回数は 
//  // n_out = t_all / dt = n_haku*_haku/(1e6/freq) [回] 
//  // 切り替え回数 n_toggle = 2*n_out = 2*n_haku*_haku/(1e6/freq)
//
//  PORTAbits.RA0 = 0; //念のため
//
//  int n_toggle = (int)(2.0*(n_haku*_haku/(1.0e6/freq)));
//  int dt_l = (int)(1.0e6/freq/2.0);
//
//  for(int i=0; i < n_toggle; i++){
//    PORTAbits.RA0 ^= 1; // 反転 
//    for(int j=0; j < dt_l; j++){
//      __delay_us(1);
//    }
//  }
//  return;
//}
// 音が体感 10倍くらい間延びする。For が遅いのではないのか。
// 決め打ちする。表計算ソフトで dt_l 計算しておく
void tone(int n_haku, int freq)
{
  PORTAbits.RA0 = 0; //念のため

  int n_toggle = (int)(2.0*(n_haku*_haku/(1.0e6/freq)));

  switch(freq){
    case _C: 
      // dt_l = 1908 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1908);
      }
      break;

    case _D: 
      // dt_l = 1701 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1701);
      }
      break;

    case _E: 
      // dt_l = 1515 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1515);
      }
      break;

    case _F: 
      // dt_l = 1433 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1433);
      }
      break;

    case _G: 
      // dt_l = 1276 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1276);
      }
      break;

    case _A: 
      // dt_l = 1136 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1136);
      }
      break;

    case _B: 
      // dt_l = 1012 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(1012);
      }
      break;

    case _CH: 
      // dt_l = 956 [us]

      for(int i=0; i < n_toggle; i++){
        PORTAbits.RA0 ^= 1; // 反転 
        __delay_us(956);
      }
      break;

  }
}

void noTone(int n_haku)
//{
//  PORTAbits.RA0 = 0; 
//  int t_muon = n_haku * _haku;
//  for(int i=0; i < t_muon; i++){
//    __delay_us(1);
//  }
//}
// 決め打ち
{
  PORTAbits.RA0 = 0; 
  __delay_ms(1000); //n_haku = 4 のパターンしかないので
}
