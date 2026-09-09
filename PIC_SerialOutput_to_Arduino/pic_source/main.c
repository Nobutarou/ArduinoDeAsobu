/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2023/11/11, 19:45
 */

// CONFIG1
/*{{{*/
// clock
// 5.2.2 に内蔵クロックなら FOSC = INTOSC
// あとは main() 内で
// 5.2.2.1 で OSCTUNEbits.TUN で調整しろ --> 工場校正済の 0b000000
// OSCCONbits.IRCF で周波数選べ --> 16MHz (32MHz 要らないよね)
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

#define _XTAL_FREQ 16000000 

// 参考
// https://hisaomemo.blogspot.com/2016/02/picarduino.html

void main(void) {
    // 16MHz clock にする設定
    OSCTUNEbits.TUN = 0b000000; // 多分要らない
    OSCCONbits.IRCF = 0b1111; // 多分要る

    // 26.5.3 によると APFCON で決めろということなので RA4 を TX とする。
    // デフォルトは RA0 だけど、ファンコンの温度センサーを RA0 に入れるつもりで設計までし
    // てしまってるので。
    APFCONbits.TXCKSEL = 1;

    //simulator でこれをしないと Ain のままになってしまう
    //しかしこれだけでは Din のままである
    ANSELA = 0; 

    // これをやると Dout になる
    TRISA = 0; 

    // このあたり 26.1.1.6 の setup に書いてないんだよ
    
    // 26.1.1.6 の set-up に従う
    //
    // 1. Baudrate を決める
    // example 26-1 によると 16MHz 駆動で 9600 [Hz] にするには SPBRG = 25 
    SPBRGL = 25;
    SPBRGH = 0;

    // 表 26-3 で Baud Rate 決定式だけど example 26-1 と同じなのは BRG16 = 0 と BRGH = 0
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;

    // 2. SYNC をクリアして(レジスタ名も書いてくれよ)
    // 表26-3 に戻るが、丁度欲しい Baud Rate 式になる。
    TXSTAbits.SYNC = 0;

    // SPEN をセットしろ (レジスタ名書いてくれよ). 1 でシリアルポートが有効
    RCSTAbits.SPEN = 1;

    // 3. 9-bit 通信が必要なら云々。とりあえず Hello World に不要だと思うので止めておこう
    // 4. TXEN ビットで送信を有効にせよ。TXIF は勝手に設定される。だそうだが TXIF が何かは
    // 今は分かっていない。
    TXSTAbits.TXEN = 1;

    // 5. interrupt したければ云々。黙って送信完了を待とう
    // 6. 9-bit 選択時の操作なのでスルー
    // 7. TXREG レジスタにデータを読ませれば、送信が初まるよ、なので、これは while ループ
    // の中でやろう

    while (1) {
        char *cmd="hello world\n";
        while(*cmd){
          while(!PIR1bits.TXIF);
          TXREG = *cmd++;
        }
        __delay_ms(1000);
    }
    return;
}

