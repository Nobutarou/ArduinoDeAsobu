/*******************************************************************************
*  slave.c - Ｉ２Ｃ通信のスレーブでのテスト                                    *
*                                                                              *
*  マスターから１バイト受信し、そのデータに＋１７足してマスターに送信する      *
*  受信したデータは１バイト毎にＬＣＤモニターに表示する                        *
*  コンパイルするにはskI2Cslave.cとskMonitorLCD.cのライブラリが必要            *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2012-01-05  きむ茶工房(きむしげ)  Create                            *
* ============================================================================ *
*  PIC 12F1822                                                                 *
*  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode           *
*******************************************************************************/

// PIC16F18323 で動かしたい
// mplab では xc8 global options -> C standard -> C90 じゃないと通らないみたい

#include <pic.h>
#include <htc.h>                        // delay用に必要
#include "skI2Cslave.h"                 // I2C関数ライブラリ用
//#include "skMonitorLCD.h"               // LCDモニター用

#define _XTAL_FREQ   8000000            // delay用に必要(クロック8MHzを指定)
                                        //
// CONFIG1
#pragma config FEXTOSC = OFF    // 内蔵水晶利用
#pragma config RSTOSC = HFINT32 // 32MHz 利用
#pragma config CLKOUTEN = OFF   // RA4 を I/O として使用
#pragma config CSWEN = ON       // NDIV でクロック落とせる
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor 使用しない

// CONFIG2
#pragma config MCLRE = OFF      // RA3 を input として利用
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT enabled, SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = OFF    // PPS は随時有効
#pragma config STVREN = ON      // Stack overflow でリセット
#pragma config DEBUG = OFF      // Debugger 不要 (使いこなせない)

// CONFIG3
#pragma config WRT = OFF        // ROM エリア書き込み OK
#pragma config LVP = ON         // これ禁止すると a-p-prog で書き込めないのでは？

// CONFIG4
#pragma config CP = OFF         // ROM エリア code protection なし。
#pragma config CPD = OFF        // EEPROP code protection なし

/*******************************************************************************
*  メインの処理                                                                *
*******************************************************************************/
void main()
{
     int ans ;

     OSCCON1 = 0b0000010; // 32MHz/4 = 8MHz
     ANSELA  = 0b00000000 ;   // アナログ使用しない(すべてデジタルI/Oに割当てる)
     TRISA   = 0b00000000 ;   // Aポートは出力のみで OK (RA3 は使えない)
     PORTA   = 0b00000000 ;   // 出力ピンの初期化(全てLOWにする)
	   //WPUA = 0;	
	   ANSELC = 0;		// アナログ入力OFF
	   TRISC = 0b00000011; // SCL, SDA の RC0, RC1 だけ input
	   PORTC = 0;
     //WPUC = 0;

     // original
     // OSCCON  = 0b01110010 ;
     // ANSELA  = 0b00000000 ;
     // TRISA   = 0b00001110 ;
     // PORTA   = 0b00000000 ;

     // これ必要だった。デフォルト設定とは？
     SSP1CLKPPS = 0b00010000; // SCL -> RC0
     RC0PPS =     0b00011000; // RC0 -> SCL
     SSP1DATPPS = 0b00010001; // SDA -> RC1
     RC1PPS =     0b00011001; // RC1 -> SDA
     
     InitI2C_Slave(8) ;       // スレーブモードでの初期化、マイアドレスは8とする

     __delay_ms(2000) ;
     //MonitorPutc(0x11) ;      // モニターの表示位置を設定する

     while(1) {
          ans = I2C_ReceiveCheck() ;              // 受信状態のチェック
          if (ans != 0) {
//               MonitorPutc(rcv_data[0]) ;         // モニターに表示する
               snd_data[0] = rcv_data[0]+0x11 ;   // 送信データをセットする
          }
     }
}
