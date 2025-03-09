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
// 式 21.1 より周波数は 31kHz くらいになる

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
#define _r1 1.0
#define _vref 3.3
#define _vbat_low 0.5
#define _vbat_high 1.2
#define _vbat_max 1.3
#define _i_low 0.1
#define _i_max 1.0

// あらかじめ ADC の値を計算しておく
#define _adc_vbat_low 155
#define _adc_vbat_high 372
#define _adc_vbat_max 403

void setup(void);
void set_duty(short duty);
void setup_led(void);

void main(void) {
    short adc; // ADC 読み値 0-255
    short duty; // duty 比 0-255
    short adc_pre=0; // 前回の ADC 読み値
    char stage=0; // トリクル 0, 通常 1
    char min=0; // 実行時間、分
    char i;
    setup();
    setup_led();

// 1サイクル 1分、120分で終了
    while(min<120){
// 止めて測ろうと思っていたけど、止めると電流流れないから 3.3V になるということに後で気が
// 付く。止めてマイナス側を測って Vdd-Vbat となってるはずだから、 1023 から引くか、止めず
// に測るか。
// 回路図、図面、秋月への買い物をしてしまったので、プラス側を止めずに測ることにする。
// 回路は、ジャンパ線で継ぎ換えるだけで変えられそうだし、ソースも 1023 から引くだけで良い
// はずだから、とりあえずやってみよう。
//      set_duty(0); //止めて測るとき
        ADCON0bits.GO_nDONE=1;
        while(ADCON0bits.GO_nDONE){};
        adc=ADRESH*256+ADRESL;
// Vbat=adc/1023*3.3
// Vr=Vdd-Vbat=3.3-adc/1023*3.3=3.3(1023-adc)/1023

// I=Vr/R=3.3(1023-adc)/1023 (Duty比100%なら)

// 電流を 0.1A にするための Duty比 (100%で 255) は
// 255:I=x:0.1 より
// x=255*0.1/I=25.5*1023/3.3/(1023-adc)
//  =7905/(1023-adc)
// 検算, 0.5V --> 155 なら x=9, I=2.8A
//       2.8*9/255=0.0988 A → OK
//
// 1A 流すには Duty 比は
// x=79050/(1023-adc)
// 検算 1.2V --> 372 なら x = 121
//      (3.3-1.2)*121/255=1.00A --> OK
        if(adc <= _adc_vbat_low){
            duty=7905/(1023-adc);
            stage=0;
        } else if(adc <= _adc_vbat_high){
            duty=79050/(1023-adc);
            stage=1;
            if (adc==adc_pre){
                set_duty(0);
                LATAbits.LATA2=0;
                while(1){};
            };
        } else if(adc <= _adc_vbat_max){
            set_duty(0);
            LATAbits.LATA2=0;
            while(1){};
        };
        set_duty(duty);
        adc_pre=adc;
        for(i=0;i<30;i++){
            __delay_ms(2000);
            // stage=0 なら反転, stage=1 なら常に 1 のつもりだが。。。
            LATAbits.LATA2=!LATAbits.LATA2 | stage;
        }
        min++;
    };
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
    PWM2DCH=duty>>2; 
    // 数値の 1ビット目をレジスタの 7ビット目に書くから
    PWM2DCL=duty<<6;
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
    T2CONbits.T2CKPS = 0;
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

// 2. ADCON0, ADCON1 の設定
//    ADCON1=| ADFM | ADCS<2:0> |-|-| ADPREF<1:0>|
//      ADFM=1 for 右詰め. 
//      ADCS=0b101 for FOSC/16
//      ADPREF = 0b00 for Vref=VDD
    ADCON1=0b11010000;

//    ADCON0=|-| CHS<4:0> | GO/DONE | ADON |
//      CHS=0b00001 for RA1
//      ADON = 1 for enable
    ADCON0=0b00000101;
/* }}} */
}
