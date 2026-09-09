/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2023/12/07, 12:36
 */

// test summary
// * 1 コマンドのサイクルに 4 周期, 32MHz/4。
// * LATA は 1サイクルで PORTA の書き換えも終わる。誤差を 0.5サイクルと考えると Hi と Low
// で 100サイクルくらい挟んでおけば、誤差は 1% 台に収まる。
// * __delay_us() は 1cycle, 0.125us 単位で待てる。
// * ADC は 99サイクル = 1.2375e-5sec 掛かる。

// とりあえずデフォルトで出してから考える。
// CONFIG1
/*{{{*/
// clock
// 5.2.2 に内蔵クロックなら FOSC = INTOSC
// あとは main() 内で
// 5.2.2.1 で OSCTUNEbits.TUN で調整しろ --> 工場校正済の 0b000000
// OSCCONbits.IRCF で周波数選べ --> 今回は timer を試したいので最速 32MHz で
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
#include <stdio.h>
#define _XTAL_FREQ 32000000 

void main(void) {
    short tmr0; // TMR0 の値を格納
    short reg; // 汎用変数
    char i; // 汎用変数
               
    // 32MHz clock にする設定. 5.2.2.6 の通りにやる
/*{{{*/
    // config FOSC で INTOSC を選んでおく (済)
  
    // これは他のソースからのコピペ。多分不要だけどおまじない。
    OSCTUNEbits.TUN = 0b000000; 

    // OSCCONbits.SCS をクリア (要らない気がする)
    OSCCONbits.SCS = 0b00; 
                          
    // IRCF を 1110。おそらく 8MHz に下がる
    OSCCONbits.IRCF = 0b1110; 

    // SPLLEN = 1 で 4倍になる。
    OSCCONbits.SPLLEN = 1;
/*}}}*/

    // 4 周期=1サイクルで 1カウンター増やす設定.Fosc/4 というやつ
    OPTION_REGbits.TMR0CS = 0; 

    // TMR0 clear 後の動作確認
/*{{{*/
//    // TMR0 clear
//    TMR0 = 0;
//    // すかさず tmr0 の値を読む。2cycle をなんらかで消費するみたいなので何回か読む
//    // 結果は MPLAB X IDE のシミュレーターの結果
//    tmr0 = TMR0; // =0 
//    tmr0 = TMR0; // =0
//    tmr0 = TMR0; // =1
//    tmr0 = TMR0; // =2
//    tmr0 = TMR0; // =3
//
//    // http://www.pcbheaven.com/picpages/Instruction_Cycle_Duration_and_Programmable_Delays/
//    // によると 4 pulse で 1 operation. はっきり言って分けがわからんが 32MHz/4 が、PIC の
//    // 1ジョブの実行時間ということかと思う。
//    // なので TMR0 クリア後、1/(32e6/4)x2 = 2.5e-7 sec はカウントできていないということに
//    // なる。
/*}}}*/

    // LATA で Hi にした後の PORTA 立ち上げ時間のテスト
    /*{{{*/
    // simulator だと while の後 tmr0=5 となる
    // 実験もしてみたけど tmr0=5 だ
    // これ while のサイクル数な可能性がある --> 本当にそうだった
    // simulator を信じると LATA の動作は 1cycle (4/32MHz=1.25e-7sec) 以内に終っているよう
    // だ。
//   while(1){
//    TRISA=0;
//    PORTA=0;
//    ANSELA=0;
//    LATA=0;
//    __delay_ms(1);
//    TMR0=0;
//    LATAbits.LATA1=1;
//    while(!PORTAbits.RA1){};
//    tmr0=TMR0;
//
//    // 実物で確かめるための serial 通信
//    // 26.1.1.6 の set-up に従う
//    // 1. Baudrate を決める
//    // example 26-1 によると 32MHz 駆動で 9600 [Hz] にするには SPBRG = 51 
//    SPBRGL = 51;
//    SPBRGH = 0;
//
//    // 表 26-3 で Baud Rate 決定式だけど example 26-1 と同じなのは BRG16 = 0 と BRGH = 0
//    BAUDCONbits.BRG16 = 0;
//    TXSTAbits.BRGH = 0;
//
//    // 2. SYNC をクリアして(レジスタ名も書いてくれよ)
//    // 表26-3 に戻るが、丁度欲しい Baud Rate 式になる。
//    TXSTAbits.SYNC = 0;
//
//    // SPEN をセットしろ (レジスタ名書いてくれよ). 1 でシリアルポートが有効
//    RCSTAbits.SPEN = 1;
//
//    // 3. 9-bit 通信が必要なら云々。とりあえず不要だと思うので止めておこう
//    // 4. TXEN ビットで送信を有効にせよ。TXIF は勝手に設定される。だそうだが TXIF が何かは
//    // 今は分かっていない。
//    TXSTAbits.TXEN = 1;
//
//    // 5. interrupt したければ云々。黙って送信完了を待とう
//    // 6. 9-bit 選択時の操作なのでスルー
//    // 7. TXREG レジスタにデータを読ませれば、送信が初まるよ、なので、これは while ループ
//    // の中でやろう
//
//    char s[5]; // serial out
//    snprintf(s, sizeof(s), "%d\n", tmr0); 
//
//    for(i=0; i<5; i++) {
//        while (!PIR1bits.TXIF);
//            TXREG = s[i];
//    }
//    __delay_ms(1000);
//   }
    //
/*}}}*/

    // LATA で Lo にした後の PORTA 立ち上げ時間のテスト
    /*{{{*/
    // simulator だと while の後 tmr0=5 でこれは while のサイクル数なだけ
    // simulator を信じると LATA の動作は 1cycle (4/32MHz=1.25e-7sec) 以内に終っているよう
    // だ。
//   while(1){
//    TRISA=0;
//    PORTA=0;
//    ANSELA=0;
//    LATAbits.LATA1=1;
//    __delay_ms(1);
//    TMR0=0;
//    LATAbits.LATA1=0;
//    while(PORTAbits.RA1){};
//    tmr0=TMR0;
//   }
/*}}}*/

    // while のテスト
    /*{{{*/
    // while だけで 5+2=7 cycle 掛かる
//   while(1){
//    TRISA=0;
//    PORTA=0;
//    ANSELA=0;
//    LATA=0;
//    __delay_ms(1);
//    TMR0=0;
//    LATAbits.LATA1=1;
//    // RA0=0 なので、ほぼ純粋に while 文の実行サイクルと言えるだろう
//    while(PORTAbits.RA0){};
//    tmr0=TMR0; // tmr0=5 になる
//    __delay_ms(1);
//   }
/*}}}*/

    // __delay_us のテスト
/*{{{*/
//    while(1){
//      TMR0 = 0;
//      __delay_us(1);
//      tmr0 = TMR0; // tmr0=6 
//
//      TMR0 = 0;
//      __delay_us(0.375);
//      tmr0 = TMR0; // tmr0=1
//
//      TMR0 = 0;
//      __delay_us(0.25);
//      tmr0 = TMR0; // tmr0=0
//    }
    // 結果考察
    // 1/(32MHz/4) = 0.125e-6 sec をきっちり待てて居ることになる。
/*}}}*/

    // ADC のテスト
    // まとめ
    // ADCに 99 cycle, 1.2375e-5 sec 掛かる。
    TRISA=0;
    LATA=0;
    ANSELA=0;

    // RA1 を AN1 とする。レイアウト考えるときにちゃんと決める
    // 16.2.6 の手順に従う
    // 1, TRIS で output 無効にして ANSEL で analog を選択
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSA1 = 1;

    // 2, clock, vref, input channel, 設定して ADC on
    // 表 16-1, Fosc=32MHz なら ADCS=0b010 の 1.0us とする。
    ADCON1bits.ADCS = 0b010;

    // ADC を AN1 に繋ぐ
    ADCON0bits.CHS = 0b00001;

    // 具体的には書いてないけど Vdd 参照で
    ADCON1bits.ADPREF = 0b00;

    //オン
    ADCON0bits.ADON = 1;

    // 3, interrupt なので無視

    // 4, acquisition time を待てだが、式 6-1 に 50℃, 10kΩのときの計算がある。
    // 繋ぐ MCP9700 は 20Ωなので、全然これ以下のはずだ。
    // 余裕を持って 50μs くらい待てば良いだろう
    __delay_ms(50);
    // 5 以降は無限ループの中へ

    // 結果は右詰めとする
    ADCON1bits.ADFM = 1;

    // 測定開始
    while(1){
        TMR0=0;
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO);
        // なぜかこの後 TMR0 が 7ずつ増える。
        // while を抜けたときの TMR0=99
        tmr0 = TMR0;
        tmr0 = TMR0;
        tmr0 = TMR0;
        tmr0 = TMR0;
    }

    return;
}
