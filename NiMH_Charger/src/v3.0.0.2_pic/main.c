// PIC Design:
// RA0 -> PWM2
// RA1 -> AN1 

// Circuit:
// Vin = 3.3V
// R1 = 1Ω

// PWM と動作周波数:
// Table 21-2 の 19.61kHz を基準に考える。 
// FOSC=8MHz で決定
// Eq. 21-4 より Duty比が 8bits = 255 となる PR2 = 63
// TMR2 プリスケールは 4倍
// 式 21.1 より波長は 0.000128s なので周波数は 7.8kHz

// ADC の設定
// Table 15-1 の FOSC=8MHz, 2.0us で行く。
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

#define _XTAL_FREQ 8000000

#define _pr2 63

// あらかじめ ADC の値を計算しておく
#define _adc_vbat_low 155
// 1.3V
#define _adc_vbat_high 403
// 1.4V
#define _adc_vbat_max 434
//#define _adc_vbat_max 900

void setup(void);
void set_duty(short duty);
void setup_led(void);

void main(void) {
    short adc; // ADC 読み値 0-255
    short duty; // duty 比 0-255
    short adc_max=0; 
    char stage=0; // トリクル 0, 通常 1
    char min=0; // 実行時間、分
    char i;
    setup();
    setup_led();
// 当初 1サイクル 1分でやっていたが、実際の電流が 0.5~0.6A といったところで 1900mAHr の電
// 池では C/3 ~ C/4 というところ。MAX713 では 168 sec でサンプリングしてるので
// 1サイクル 3分、120分で終了
    while(min<120){
        set_duty(0); 

        //PWM 周期が 0.000128sec。10倍掛けて安全に止める（なんとなくのイメージ)として
        __delay_ms(2);
        ADCON0bits.GO_nDONE=1;
        while(ADCON0bits.GO_nDONE){};
        adc=1023-(ADRESH*256+ADRESL);
//
        if(adc <= 155){
        // adc=155=0.5V
        // 0V で 3.3A。0.1A 狙いで duty=3.0%=8
        // 0.5V で 2.8A。duty=8 で 0.088A
            duty=8;
            stage=0;
        } else if(adc <= 310){
        // adc=310=1.0V
        // 0.5V で 2.8A。1A 狙いで duty=36%=92
        // 1V で 2.3A。2.3*92/255=0.83A
            duty=92;
            stage=1;
        } else if(adc <= 434){
        // adc=434=1.4V
        // 1V で 2.3A。1A 狙いで duty=1/2.3*255=111
        // 1.4V で 1.9A。1.9*111/255=0.83A
            duty=111;
            stage=1;
        } else if(adc <= 1023){
        // adc=1023=3.3V
        // 1.4V で 1.9A。1A 狙いで duty=1/1.9*255=134
        // 3.3V で 0A。
            duty=134;
            stage=1;
        } else {
        // error
          set_duty(0);
          while(1){
            __delay_ms(1000);
            LATAbits.LATA2=1;
            __delay_ms(500);
            LATAbits.LATA2=0;
          };
        };
        // これのどこにもない PWM のパターンとなった。
        // PWM 掛ける瞬間にシャットダウンしてる？
        // 抵抗？
       
        if (adc >= _adc_vbat_high && adc <= adc_max){
            set_duty(0);
            LATAbits.LATA2=0;
            while(1){
              __delay_ms(100);
              LATAbits.LATA2=1;
              __delay_ms(100);
              LATAbits.LATA2=0;
            };
        };
        if(adc >= _adc_vbat_max){
            set_duty(0);
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
    //120分経過につき終了
    set_duty(0);
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

// 8MHz 動作
// OSCCON = |-| IRCF<3:0> | - | SCS<1:0? |
//   IRCF=1110=8MHz
//   SCS not needed
    OSCCON = 0b01110000;

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
//   T2CON の T2CKPS プリスケーラをセット --> 1.0 倍 = 0b00
//   4倍 0b01
//   1倍だと波形がちゃんと出ない。
    T2CONbits.T2CKPS = 0b01;
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
//      ADCS=0b101 for FOSC/16
//      ADCS=0b010 for FOSC/32
//      ADPREF = 0b00 for Vref=VDD
    ADCON1=0b10100000;

//    ADCON0=|-| CHS<4:0> | GO/DONE | ADON |
//      CHS=0b00001 for RA1
//      ADON = 1 for enable
    ADCON0=0b00000101;
/* }}} */
}
