/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2024/05/19, 20:27
 */
// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with 2x PLL (32MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = ON        // Watchdog Timer Enable bits (WDT enabled, SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

#include <xc.h>

#define _XTAL_FREQ 8000000

#define V_0degC 500.0f // [mV]
#define T_C     10.0f  // [mV/℃]
#define V_REF   1024.0f // [mV]

// 設計
// V_OUT ピンは RA4 に接続

void main(void) {
  // クロック設定
  // OSCCON1
  // | - | NOSC,2:0> | NDIV<3:0> |
  // NOSC = 000: 16MHz x 2PLL = 32MHz
  // NDIV = 0010: 32MHz/4 = 8MHz
  OSCCON1 = 0b00000010;

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
  ADCON1bits.ADCS = 0b101; //表22-1 より 8MHz 駆動での推奨範囲
  FVRCONbits.FVREN = 1; //内蔵参照電圧の利用
  FVRCONbits.ADFVR = 0b01; //1024[mV] を利用

  //FVRCON の FVRRDY はこの機種では常時 1 なので待つ必要はない
  // while(!FVRCONbits.FVRRDY) {};

  ADCON0bits.CHS = 0b000100; // RA4 を ANA4 に指定
  ADCON1bits.ADPREF = 0b11; // 参照電圧を FVR に指定
  ADCON0bits.ADON = 1; // ADC 有効化

  // ADON が有効になるのを待つ必要がある。式 22-1 によると usec のオーダーなので 1ms 待て
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

