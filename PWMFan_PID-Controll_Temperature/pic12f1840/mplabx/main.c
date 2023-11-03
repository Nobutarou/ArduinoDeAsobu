/*
 * File:   main.c
 * Author: snob
 *
 * Created on 2023/11/01, 22:50
 */

// とりあえずデフォルトで出してから考える。
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
#define _XTAL_FREQ 16000000 
#define _max_pwm 200 // 最大 pwm dc (100% のときの値) 
#define _min_pwm 30 // 最小 pwm dc。止めない。
#define _vref 2048.0 // Vref [mV]
#define _v0c  500.0 // 電圧 at 0℃ [mV]
#define _d_v  10.0 // 電圧/温度の傾き [mV/℃]
#define _target_temp 45.0 // 目標温度 [℃]
#define _crit_temp 80.0 // 許容最大温度 [℃]
#define _dt 1000.0 // タイムステップ [ms]

void main(void) {
    // 16MHz clock にする設定
    OSCTUNEbits.TUN = 0b000000; // 多分要らない
    OSCCONbits.IRCF = 0b1111; // 多分要る

    // 温度センサーセットアップ
    /*{{{*/
    unsigned short temp_pin_input; // analog input from 温度センサー
    float volt; // 電圧
    float temp; // 温度。可読性のために、ここは変数使う。

    // RA0 を AN0 とする。レイアウト考えるときにちゃんと決める
    // 16.2.6 の手順に従う
    // 1, TRIS で output 無効にして ANSEL で analog を選択
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;

    // 2, clock, vref, input channel, 設定して ADC on
    // 表 16-1, 16MHz なら Fosc/64 -> 4μs で良いそうだ
    ADCON1bits.ADCS = 0b110;

    // Vref は内蔵 2.048V
    FVRCONbits.FVREN = 1; // 内蔵Vref 有効化
    FVRCONbits.ADFVR = 0b10;
    while (!FVRCONbits.FVRRDY) {
    }; // 有効化を待つ

    // ADC を AN0 に繋ぐ
    ADCON0bits.CHS = 0b00000;

    // 具体的には書いてないけど Internal Vref 参照で
    ADCON1bits.ADPREF = 0b11;

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

    /*}}}*/

    // PWM セットアップ
    /*{{{*/
    // 普通の PWM は CCP1 ピンを使う。RA2 か RA5 に APFCON で指定
    // とりあえず RA2 にしておく
    // おそらく初期値であり、そのため 24.3.2 の手順に書いてないのだろう
    APFCONbits.CCP1SEL = 0;

    // 式 24-1 より PR 
    // PR2 = PWM_Period/4*FOSC/TMR2_Prescale_Value -1
    // PWM_Period = 5e-5[s]
    // FOSC=16e6[1/s]
    // TMR2_Prescale_Value は 1, 4, 16, 64
    // TMR2_Prescale_Value = 1 なら PR2 = 199
    // TMR2_Prescale_Value = 4 なら PR2 = 49
    // で分解能は式 24-4 で
    // PR2 = 49 で約 8ビット弱の 199 になるので十分。
    // PWM 100% のときの DC は 式 24-3 より
    //   DC = 4(PR2+1)*1.0 = 200

    // 24.3.2 の手順に従う
    // 1, TRIS で CCP1 output を無効にする
    TRISAbits.TRISA2 = 1;

    // 2, PR2 を指定
    PR2 = (_max_pwm-1)/4;

    // 3, CCP1CON で CCP1 を PWM モードにする設定みたいな雰囲気
    // active-high 
    CCP1CONbits.CCP1M = 0b1100;

    // P1A のみ (CCP1 と同じものを示してると思う) PWM
    CCP1CONbits.P1M = 0;

    // 4, duty cycle を指定する
    // CCP1CONbits.DC1B1 が下位 2bits
    // CCPR1L が上位 8bits とややこしい
    // とりあえず 100% とする
    CCPR1L = _max_pwm / 4;
    CCP1CONbits.DC1B = _max_pwm % 4;

    // 5, Timer2 の設定と有効化
    // 分かってないが書いてある通りに。
    PIR1bits.TMR2IF = 0;

    // TMR2_prescale_value = 4
    T2CONbits.T2CKPS = 0b01;

    // timer enable
    T2CONbits.TMR2ON = 1;

    //6, PWM output pin enable
    // timer overflow なるものを待つ。分かってないが書いてある通りにやる
    while (!PIR1bits.TMR2IF) {
    };

    // CCP1 pin output を有効にする
    TRISAbits.TRISA2 = 0;

    /*}}}*/

    // PID セットアップ
    /*{{{*/
    float diff_temp; // 温度差 [K]
    float d_temp; // 温度変化 [K]
    float pre_temp; // 前サイクルの温度 [℃]
    float int_diff_temp = 0.0; // diff_temp の時間積分
    int pwm_dc; // 計算中におかしな数字になる可能性があるので int
    const float kp = _max_pwm / (_crit_temp - _target_temp); // 最大温度で最大 PWM
    const float kd = _max_pwm / (10.0 / _dt); // 1サイクルで 10℃上昇で最大 PWM
    const float ki = 0.0; // 積分項も可読性のために残しておく
    /*}}}*/

    while (1) {
        // 16.2.6, 5 から
        // 測定開始
        // お前は GO か GO_nDONE なのか。機種ごとに違うの止めろ
        ADCON0bits.GO = 1;
        // 6, 測定完了を待つ
        while (ADCON0bits.GO) {
        };
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
        CCPR1L = pwm_dc / 4;
        CCP1CONbits.DC1B = pwm_dc % 4;

        __delay_ms(_dt);

    }

    return;
}
