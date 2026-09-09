/*
 * File:   main.c
 * Author: snob
 *
 * Created on May 18, 2024, 4:54 PM
 */

// 結果
// 割り込みできていない

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

// table 5.1 によるとこれで 16MHz にできる
#pragma config PLLEN = 1  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config CPUDIV = CLKDIV3 // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low Speed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multiplier Selection Bit (3x Output Frequency Selected)

#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 16000000

// ピン設計, 回路
// RA5 を VP, RA4 を VN
// VN に 22kΩ。これで 2.75V, 0.75V のパルスとなるはず
// VDD は 5V
// RC0~5 を LED 出力とする。全部使ってしまえばビット操作不要になるはず
// #define で上手にやる技術がないので直接打ち込む

unsigned int count;

void main(void) {
  unsigned int final_count;
  double temp;

  OSCCON = 0b11111100; //PLL 3x, 16MHz のはず
  // ピン初期化
  //ANSELA = 0;
  TRISA = 0;
  PORTA = 0;
  TRISAbits.TRISA4 = 1;

  ANSELC = 0;
  TRISC = 0;
  PORTC = 0;

  // GPIO ピンの割り込み有効
  INTCONbits.GIE = 1; 
  INTCONbits.IOCIE = 1;

  while(1){
    count = 0;
    
    IOCANbits.IOCAN4 = 1; //RA4 の falling 割り込み開始
    PORTAbits.RA5 = 1;
    __delay_ms(104);
    final_count = count;
    IOCANbits.IOCAN4 = 0; // 割り込み完了
    PORTAbits.RA5 = 0;

    temp=(final_count/4096.0*256.0)-50.0;

    PORTC = (int)temp; //勝手に RC0 -> RC5 に向かって 6ビットで光る(はず)

    __delay_ms(1000);
  }
  return;
}

void __interrupt() isr(void) {
  count++; 
}
