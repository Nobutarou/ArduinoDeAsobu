// PIC Design:
// RA0 -> PWM2
// RA1 -> AN1 

// Circuit:
// Vin = 5.0V
// R1 = 1Ω

// FOSC=4MHz で十分じゃない？
// 稼働部品も表面実装セラコンもないので鳴きもないでしょう。

// PWM: 
// せっかくだから NiMH のときとは、いろいろ変更して遊んでみよう。
// 0~100% と考えると PR2=24 が分かりやすい。
// TMR2 プリスケールは 2倍以上が必要だ。今回は 16倍にしてみよう。
// 式 21-1 より PWM period=8e-4 sec → 2.5kHz 
// on, off の high, low 切替えられるけど、まあしなくていいや。

// ADC の設定
// Table 15-1 の FOSC=4MHz, 4.0us で行く。
// ADCS=0b101 (FOSC/16)  となる。

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include <xc.h>

#define _XTAL_FREQ 4000000

#define _pr2 24

// あらかじめ ADC の値を計算しておく
// 標準 3.7V --> 757.02
#define _adc_vbat_high 758
// 3.8V --> 777.48
#define _adc_vbat_max 778

void setup(void);
void set_duty(short duty);
void setup_led(void);

void main(void) {
    short adc; // ADC 読み値 0-255
    short duty; // duty 比 0-255
    short adc_max=0; 
    char stage=0; // 0: トリクル、1: 通常
    char min=0; // 実行時間、分
    char i;
    setup();
    setup_led();
    
    // p-MOS は逆流するので PIC が電源側でオンにする前に動いてしまう。
    // ADC に Vcc より大きな入力が入りちゃんと動かない。
    // 1, 電池を外しておく
    // 2, 電源オン
    // 3, 電池を繋ぐ
    // ための 10sec
    set_duty(100); 
    __delay_ms(10000);

    while(min<120){
        // NiMH の経験から、どうせ電流は想定ほど流れない
        // 1サイクル 2分にしておこう
        
        // P-MOS は High で止まる。
        set_duty(100); 

        // PWM の周期が 2.5ms なので、なんとなく一回 PWM をきっちり休ませる。
        // 意味は分からん
        // ゲート電荷が 5V 利用なら 60nC で、電流が 0.016A の予定なので、電流に変化がなけ
        // れば 3us で切り替わるので 10ms は、これの妨げにはならない。
        __delay_ms(10);
        ADCON0bits.GO_nDONE=1;
        while(ADCON0bits.GO_nDONE){};
        adc=ADRESH*256+ADRESL;

        if(adc <= 552){
        // adc=552 → 2.7V
        // 標準の 3.7V より 1V 低い。これより低くなるまで使うことのないだろう。
        // 2.7V なら電流は 2.3A。デューティ比 5% で 2.3Ax0.05=0.11A
        // 仮に 0V でも 5Ax0.05=0.25A
        // まあ良いでしょう。Pch なので (100-5)=95%
            duty=95;
            stage=0;
        } else if(adc <= 634){
        // adc=634 → 3.1V
        // 2.7V なら 2.3A なのでデューティ比 43% で 2.3Ax0.43=0.99A
        // 3.1V なら 1.9A なので 1.9Ax0.43=0.82A
        // 100-43=57%
            duty=57;
            stage=1;
        } else if(adc <= 696){
        // adc=696 → 3.4V
        // 3.1V なら 1.9A なのでデューティ比 53% で 1.9Ax0.53=1.01A
        // 3.4V なら 1.6A なので 1.6Ax0.53=0.85A
        // 100-53=47
            duty=47;
            stage=1;
        } else if(adc <= 757){
        // adc=757 → 3.7V
        // 3.4V なら 1.6A なのでデューティ比 63% で 1.6Ax0.63=1.01A
        // 3.7V なら 1.3A なので 1.3Ax0.63=0.82A
        // 100-63=37
            duty=37;
            stage=1;
        } else if(adc <= 1023){
        // adc=1023 → 5V
        // 3.7V なら 1.3A なのでデューティ比 77% で 1.3Ax0.77=1.00A
        // 5.0V なら 0A なので 流れない
        // 100-77=23
            duty=23;
            stage=1;
        } else {
        // error
        // ここには来ないはずだけど ADC が異常値を出すと来るかもしれない
          set_duty(100);
          while(1){
            __delay_ms(1000);
            LATAbits.LATA2=1;
            __delay_ms(500);
            LATAbits.LATA2=0;
          };
        };
       
        // 電圧十分かつ一定につき完了
        if (adc >= _adc_vbat_high && adc <= adc_max){
            set_duty(100);
            LATAbits.LATA2=0;
            while(1){
              __delay_ms(100);
              LATAbits.LATA2=1;
              __delay_ms(100);
              LATAbits.LATA2=0;
            };
        };
        // 電圧十分につき完了
        if(adc >= _adc_vbat_max){
            set_duty(100);
            LATAbits.LATA2=0;
            while(1){
              __delay_ms(500);
              LATAbits.LATA2=1;
              __delay_ms(1000);
              LATAbits.LATA2=0;
            };
        };
        set_duty(duty);
        if(adc > adc_max){
            adc_max=adc;
        };
        for(i=0;i<30;i++){
            __delay_ms(2000);
            LATAbits.LATA2=1;
            __delay_ms(2000);
            LATAbits.LATA2=stage;
        }
        min=min+2;
    };
    // 120分で終了
    set_duty(100);
    LATAbits.LATA2=0;
    while(1);
    return;
}

void setup_led(void){
    PORTAbits.RA2=0;
    TRISAbits.TRISA2=0;
    ANSELAbits.ANSA2=0;
    LATAbits.LATA2=0;
}

void set_duty(short duty){
    // 左詰めの 10ビット表記だから、上位 8ビットは下 2ビットを捨てる。
    PWM2DCH=duty >>2; 
    // 数値の 1ビット目をレジスタの 7ビット目に書くから
    PWM2DCL=(duty % 4) <<6;
    return;
}

void setup(void){
/* {{{ */
// 初期設定
// FOSC, PWM, ADC の設定

// 4MHz 動作
// OSCCON = |-| IRCF<3:0> | - | SCS<1:0? |
//   IRCF=4MHz=1101
//   SCS not needed
    OSCCON = 0b01101000;

// RA0 の PWM2 準備
// 基本 21.1.9 の通りにやって
// http://zattouka.net/GarageHouse/micon/MPLAB/12F1501/memo.htm 
// で不明点を補完していく感じで。
// 1, TRISA で TRISA0 を Input にする
    TRISAbits.TRISA0=1;
// 2, PWM2CON レジスタをクリア
    PWM2CON = 0;
// 3, PR2 レジスタを指定
    PR2= _pr2 ;
// 4, PWM2DCH レジスタと PWM2DCL <7:6> をクリアする。などと言っているが PWM2DCL は <7:6>
// しか存在しないので丸ごとクリアできるはず
    PWM2DCH=0;
    PWM2DCL=0;
// 5, PIR1 の TMR2IF をクリア
    PIR1bits.TMR2IF=0;
//   T2CON の T2CKPS プリスケーラ
//   16倍 0b10
//   1倍だと波形がちゃんと出ない。
    T2CONbits.T2CKPS = 0b10;
//   T2CON の TMR2ON を enable = 1
    T2CONbits.TMR2ON = 1;
// 6, PIR1 の TMR2IF が 1 になるのを待て
    while(!PIR1bits.TMR2IF){};
// 7, TRISA0 をアウトプットにしろ
    TRISAbits.TRISA0=0;
//   PWN2CON の PWM2OE をセットしろ
    PWM2CONbits.PWM2OE=1;
// 8,PWM2CON レジスタを適切に設定しろ。「適切」が分かれば苦労しないんだが。
//   多分これで、PWM2 が始まったはず。
//   ただし PWM2DC{H,L}=0 なのでデューティ比 0 で出力無し状態のはず。
    PWM2CONbits.PWM2EN = 1;

// RA1 の ADC 準備
// 15.2.6 の Procedure に従う
//    ANSEL でアナログピン指定
    TRISAbits.TRISA1=1;
    ANSELAbits.ANSA1=1;

// pull-up 無効
// やらないと 80% くらいの値を常時読むことになってしまう
    OPTION_REGbits.nWPUEN=1;
    WPUAbits.WPUA1=0; //多分不要

// 2. ADCON0, ADCON1 の設定
//    ADCON1=| ADFM | ADCS<2:0> |-|-| ADPREF<1:0>|
//      ADFM=1 for 右詰め. 
//      ADCS=0b010 for FOSC/32
//      ADPREF = 0b00 for Vref=VDD
    ADCON1=0b10100000;

//    ADCON0=|-| CHS<4:0> | GO/DONE | ADON |
//      CHS=0b00001 for RA1
//      ADON = 1 for enable
    ADCON0=0b00000101;
/* }}} */
}
