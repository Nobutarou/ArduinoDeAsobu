/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2024/05/19, 17:49
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = CLKDIV6 // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low Speed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multiplier Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include <xc.h>

// 水晶を 16MHz, PLL x 3 で 48MHz, CPUDIV で /6 して 8MHz となる予定
#define _XTAL_FREQ 8000000 

#define V_0degC 500.0 // [mV]
#define T_C     10.0  // [mV/℃]
#define V_REF   1024.0 // [mV]

// 設計
// V_OUT ピンは RA4 に接続

void main(void) {
  // クロック設定
  // | SPLLEN | SPLLMULT | IRCF<3:0> | SCS<1:0> |
  // SPLLEN = 1: PLL enable
  // SPLLMULT = 1: 3x
  // IRCF = 1111: 16MHz
  // SCS = 10: 内蔵クロック
  OSCCON=0b11111111;  

  // 変数設定
  unsigned short V_OUT_ADC; // V_OUT ピンの ADC 値 
  float V_OUT; // V_OUT ピンの電圧 [mV]
  float T_A;   // 測定温度 [℃]

  // RC ピンの初期化
  ANSELC = 0;
  TRISC = 0;
  PORTC = 0;

  // RA4 の ADC 準備
  TRISAbits.TRISA4 = 1;
  ANSELAbits.ANSA4 = 1;
  ADCON1bits.ADCS = 0b101; //表16-1 より 8MHz 駆動での推奨範囲
  FVRCONbits.FVREN = 1; //内蔵参照電圧の利用 
  FVRCONbits.ADFVR = 0b01; //1024[mV] を利用

  //FVRCON の FVRRDY はこの機種では常時 1 なので待つ必要はない
  // while(!FVRCONbits.FVRRDY) {};

  ADCON0bits.CHS = 0b00011; // RA4 を AN3 に指定
  ADCON1bits.ADPREF = 0b11; // 参照電圧を FVR に指定
  ADCON0bits.ADON = 1; // ADC 有効化
                       
  // ADON が有効になるのを待つ必要がある。式 16-1 によると usec のオーダーなので 1ms 待て
  // ば十分であろう 
  __delay_ms(1); 
                 
  ADCON1bits.ADFM = 1; //結果は右詰め。計算が楽だから

  while(1){
    ADCON0bits.GO = 1; // ADC 開始
    while(ADCON0bits.GO){}; // ADC 完了を待つ
    V_OUT_ADC = (ADRESH << 8) + ADRESL; //*256 でも良いんだけどレジスタ操作の方がオサレ
    V_OUT = V_OUT_ADC/1023.0 * V_REF; // V_REF=1024 なら、ほぼ ADC の値そのままでも良いん
                                      // だけどね
    T_A= (V_OUT - V_0degC)/T_C;
    PORTC = (int)T_A; 
    __delay_ms(1000);
  }

  return;
}
