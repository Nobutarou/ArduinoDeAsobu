/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2023/10/30, 21:17
 */

/* hardware setup
温度センサー --> RC0
PWM --> RA2

設計しながら様子を見る
*/

#include <xc.h>
#define _XTAL_FREQ 32000000 // データシートが正しくて、理解も正しければ 16MHz 動作なんだが
#define _max_pwm 99 // 最大 pwm dc 
#define _min_pwm 15 // 最小 pwm dc。止めない。
#define _vref 2048.0 // Vref [mV]
#define _v0c  500.0 // 電圧 at 0℃ [mV]
#define _d_v  10.0 // 電圧/温度の傾き [mV/℃]
#define _target_temp 45.0 // 目標温度 [℃]
#define _crit_temp 80.0 // 許容最大温度 [℃]
#define _dt 1000.0 // タイムステップ [ms]

// CONFIG1
/*{{{*/
#pragma config FEXTOSC = OFF    // 内蔵クロック使う
#pragma config RSTOSC =  HFINTPLL // 9.2.2.1 に 001 で 32MHz と書いてあるんだよなあ
// REGISTER 9-6: OSCFRQ 読むと、この設定は 16MHz x 2 っ
// ていう設定のようだなあ
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o
// or oscillator function on OSC2)
#pragma config CSWEN = OFF      // クロックを切り替える必要はない
#pragma config FCMEN = OFF      // 外部クロックが壊れても内部クロックで動かす機能だけど不要

// CONFIG2
#pragma config MCLRE = ON       // これ無効にしちゃうと FT232R での書き換え不可能にならな
// いかな
#pragma config PWRTE = ON       // 電源が安定するまで稼働を待つんでしょ。ON で良くね。
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_0 // 
#pragma config WDTE = OFF        // 少なくとも今は使いこなせない
#pragma config WDTCWS = WDTCWS_0// 
#pragma config WDTCCS = LFINTOSC // WDT input clock selector (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

/*}}}*/

void main(void) {
    // 温度センサーセットアップ
    /*{{{*/
    unsigned short temp_pin_input; // analog input from 温度センサー
    float volt; // 電圧
    float temp; // 温度。可読性のために、ここは変数使う。

    // RC0 を ANC0 として使おう
    // ADPREF をどこに入れられるのか今のところ分からない
    TRISCbits.TRISC0 = 1; // RC0 は input
    ANSELCbits.ANSC0 = 1; // RC0 は analog input 
    ADCON0bits.CHS = 0b010000; // Analog channel selection = RC0
    ADCON1bits.ADPREF = 0b11; // 内蔵 2.048V を使う。外部 Vref 不要になった
    FVRCONbits.ADFVR = 0b10; // 2.048V. 20.1.3 節で説明のない 1.024V ってのもある. こうい
    // うの本当にどっちが正しいのか何とかしてくれよ。
    FVRCONbits.FVREN = 1; // 内蔵参照電圧有効化
    ADCON1bits.ADCS = 0b110; // 表 20-1 によると 32MHz だと 1/64 しても 2μsec で十二分に
    // 速い
    ADCON1bits.ADFM = 1; // 10bit の ADRES は右詰め
    ADCON0bits.ADON = 1; // ADC を有効化

    while (!FVRCONbits.FVRRDY) {
    }; // 内蔵参照電圧のスタートアップを待つ
    /*}}}*/

    // PWM セットアップ
    /*{{{*/
    // pin は RA2 にしよう. 他の機能と混じってないから. PWM は 3-6 どれでも良いみたいなの
    // で 3 にしておく
    T2CLKCONbits.CS = 0b0010; // FOSC system clock 32MHz のつもりだけど 
    TRISAbits.TRISA2 = 1; // 29.1.9 によると output を無効にするんだそうだ。input にしろっ
    // て書いてくれんか？
    PWM3CONbits.PWM3POL = 0; // データシートに書いてないと思うけど従っておく。普通にアクテ
    // ィブで Hi を出す
    PR2 = 99; // 2kHz となる
    PWM3DCL = (99 % 4) * 64; // 7, 8bit目に書くので 2^6=64 を掛ける必要がある
    PWM3DCH = 99 / 4; // 
    PIR4bits.TMR2IF = 0; // 意味不明。29.1.9 の step 5 に従う。
    T2CONbits.CKPS = 0b010; // TMR2_prescale = 4.0 の意図だけど
    T2CONbits.ON = 1; // Timer2 有効
    while (!PIR4bits.TMR2IF) {
    }; // TMR2IF が立つまで待てと step 6 が言っている
    TRISAbits.TRISA2 = 0; // step 7 で output に指定
    RA2PPS = 0x0B; // 表15-4 で RA2 ピンが PWM3OUT。
    PWM3CONbits.PWM3EN = 1; // PWM module 有効化

    /*}}}*/

    // PID セットアップ
    float diff_temp; // 温度差 [K]
    float d_temp; // 温度変化 [K]
    float pre_temp; // 前サイクルの温度 [℃]
    float int_diff_temp = 0.0; // diff_temp の時間積分
    int pwm_dc; // 計算中におかしな数字になる可能性があるので int
    const float kp = _max_pwm / (_crit_temp - _target_temp); // 最大温度で最大 PWM
    const float kd = _max_pwm / (10.0 / _dt); // 1サイクルで 10℃上昇で最大 PWM 
    const float ki = 0.0; // 積分項も可読性のために残しておく

    while (1) {
        // 温度測定
        ADCON0bits.GOnDONE = 1; // ADC 開始
        while (ADCON0bits.GOnDONE) {
        }; // 変換終了を待つ
        temp_pin_input = ADRESH * 256 + ADRESL;

        // 温度計算
        volt = temp_pin_input / 1023.0 * _vref;
        temp = (volt - _v0c) / _d_v;
        diff_temp = temp - _target_temp;
        d_temp = temp - pre_temp;
				//int_diff_temp = int_diff_temp + diff_temp * _dt;
        //積分数値あふれとかどうなんのか知らないから計算しないでおく pre_temp = temp;

        pre_temp = temp;

        // PID 計算
        pwm_dc = kp * diff_temp + kd * d_temp / _dt + ki * int_diff_temp;
        
        if (pwm_dc > _max_pwm) {
            pwm_dc = _max_pwm;
        } else if (pwm_dc < _min_pwm) {
            pwm_dc = _min_pwm;
        };

        // PWM 変更
        PWM3DCL = (pwm_dc % 4) * 64;
        PWM3DCH = (pwm_dc / 4);

        __delay_ms(_dt);

    }
}

