// 速度が必要な用途のために、いろいろ計測してみる

// define
/* {{{ */
#define DEBUG 1    // デバグ出力不要なら 0
#if DEBUG
#define D_Begin(...) Serial.begin(__VA_ARGS__);
#define D_print(...)    Serial.print(__VA_ARGS__)
#define D_write(...)    Serial.write(__VA_ARGS__)
#define D_println(...)  Serial.println(__VA_ARGS__)
#else
#define D_Begin(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif
/* }}} */

void setup() {
  //definition
  //汎用
  unsigned int cnt0; // 16ビット整数用
  unsigned int cnt1; // 16ビット整数用
  unsigned int an0; // 16ビット整数用
  unsigned int an1; // 16ビット整数用
  unsigned long t0; // 32ビット整数用
  unsigned long t1; // 32ビット整数用
  boolean b0;
  boolean b1;
  //結果格納用

  Serial.begin(9600);

  // Timer TCB2 に関する設定を調査
  /* {{{ */
  D_print("CLKCTRL= ");
  D_print(CLKCTRL.MCLKCTRLB,BIN);
  D_println();
  //CLKCTRL=0 なので CLK_PER = クロックそのもので、それがタイマー TCB2 にも向かう。

  D_print("TCB2.CTRLA= ");
  D_print(TCB2.CTRLA,BIN);
  D_println();
  //CTRLA=0b11 なので CLSEL=001 なので CLK_PER/2 でタイマーがカウント。つまり 2サイクルに
  //一回カウントが進む。
  //CTRLA=0b01 とすると 1サイクル 1カウントになるけど、どの関数がどこで使っているか知らな
  //いので触らない。
/* }}} */
 
  // micros() 測定
/* {{{ */
  TCB2.CNT=0; //桁溢れに対応する実力がないのでタイマーをリセット
  cnt0=TCB2.CNT;
  t0=micros();
  t1=micros(); //最適化で勝手に縮められたくないので別の変数にしておく。 
  cnt1=TCB2.CNT;

  Serial.print("t1-t0= "); 
  Serial.print(t1-t0); //使わないと warning が出るし、最適化ですっ飛ばされても嫌だ。
  Serial.print(", micros(): ");
  Serial.print(cnt1-cnt0);
  Serial.println();

/* }}} */

  // digitalWrite の High を計測
/* {{{ */
  pinMode(PIN_PA7,OUTPUT);
  pinMode(PIN_PC0,OUTPUT); //同じピンだと最適化で纏められたら恐いので別のピンにしておく

  // もしかしたらピン毎に速度が違うとか、High -> High と Low -> High で違うとかあるかもだ
  // けど、無視

  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  digitalWrite(PIN_PA7,HIGH);
  digitalWrite(PIN_PC0,HIGH);
  cnt1=TCB2.CNT;

  Serial.print("digitalWrite(PIN_xxx,HIGH): ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // digitalWrite の Low を計測
/* {{{ */
  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  digitalWrite(PIN_PA7,LOW);
  digitalWrite(PIN_PC0,LOW);
  cnt1=TCB2.CNT;

  Serial.print("digitalWrite(PIN_xxx,LOW): ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // PORTx.OUT = PORTx.OUT | 0bxxxxxxxx を計測
/* {{{ */
  // PA7, PC0 を High へ切り替え
  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  PORTA.OUT=PORTA.OUT | 0b10000000;
  PORTC.OUT=PORTC.OUT | 0b00000001;
  cnt1=TCB2.CNT;

  Serial.print("PORTx.OUT=PORTx.OUT | 0bxxxxxxxx: ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // PORTx.OUT = PORTx.OUT & 0bxxxxxxxx を計測
/* {{{ */
  // PA7, PC0 を Low へ切り替え
  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  PORTA.OUT=PORTA.OUT & 0b01111111;
  PORTC.OUT=PORTC.OUT & 0b11111110;
  cnt1=TCB2.CNT;

  Serial.print("PORTx.OUT=PORTx.OUT & 0bxxxxxxxx: ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // PORTx.OUT = 0bxxxxxxxx を計測
/* {{{ */
  D_print("PORTA.OUT=");
  D_print(PORTA.OUT,BIN);
  D_println();
  D_print("PORTC.OUT=");
  D_print(PORTC.OUT,BIN);
  D_println();
  // ここまでの時点でどちらも 0

  // digitalWrite の High も Low も大差なかったから、High 側だけでいいや。
  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  PORTA.OUT=0b10000000;
  PORTC.OUT=0b00000001;
  cnt1=TCB2.CNT;

  Serial.print("PORTx.OUT=0bxxxxxxxx: ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // digitalRead() を計測
/* {{{ */
  pinMode(PIN_PA7,INPUT);
  pinMode(PIN_PC0,INPUT); 

  // もしかしたらピン毎に速度が違うとか、High -> High と Low -> High で違うとかあるかもだ
  // けど、無視

  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  digitalRead(PIN_PA7);
  digitalRead(PIN_PC0);
  cnt1=TCB2.CNT;

  Serial.print("digitalRead(): ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // (PORTA.OUT >> 7) & 1, (PORTC.OUT >> 1) & 1  を計測
/* {{{ */
  pinMode(PIN_PA7,INPUT);
  pinMode(PIN_PC1,INPUT); //PC0 だとビットシフトが不要だから PC1 にする

  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  b0=(PORTA.OUT>>7) &1; //何かに使わないと省かれるかもだから
  b1=(PORTC.OUT>>1) &1; //何かに使わないと省かれるかもだから
  cnt1=TCB2.CNT;

  Serial.print("b0= ");
  Serial.print(b0);
  Serial.print(", b1= ");
  Serial.print(b1);
  Serial.print(", (PORTx.OUT >> n)&1: ");
  Serial.print(cnt1-cnt0);
  Serial.println();
/* }}} */

  // analogRead() の計測 (12ビット最速）

  analogReadResolution(12);
  analogClockSpeed(20000);
  analogSampleDuration(0);

  TCB2.CNT=0; 
  cnt0=TCB2.CNT;
  an0=analogRead(PIN_PD7);
  an1=analogRead(PIN_PC0);
  cnt1=TCB2.CNT;

  Serial.print("an0= "); //最適化で消されない対策
  Serial.print(an0);
  Serial.print(", an1= ");
  Serial.print(an1);
  Serial.print(", analogRead(): ");
  Serial.print(cnt1-cnt0);
  Serial.println();
}

void loop() {
}

