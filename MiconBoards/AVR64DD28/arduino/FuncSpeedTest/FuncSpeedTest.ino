// 速度が必要な用途のために、いろいろ計測してみる

void setup() {
  //definition
  //汎用
  unsigned long t0; //for micros()
  unsigned long t1; //for micros()
  unsigned long t2; //for micros()
  unsigned long t3; //for micros()
  unsigned short i; //for while()

  //専用
  float Tmicros; //micros の時間。
  float Twhile; //while() の時間。割り算なので float
  float Tinc; //i++ の時間
  float Tinput; //digitalRead() の時間：

  Serial.begin(9600);

  //まずは micros() の計測をする。
 /* {{{ */
  //通常、このように micros() 二個で目的の関数を挟んで調べる。なので t2-t1 が micros() 自
  //身の遅延ということになる
  //while() や i++ の速度が不明なので、ここでは使えない。
  //1回: 5us
  //10回: 4.9000
  //22回: 4.7727
  //47回: 4.7872
  //100回: 4.7700
  t0=micros();
/* {{{ */
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
  t1=micros();
/* }}} */
  Tmicros=float(t1-t0)/100.0;

  // Serial.print("t0=");
  // Serial.print(t0);
  // Serial.print(", t1=");
  // Serial.print(t1);

  Serial.print(", Tmicros=");
  Serial.print(Tmicros,4);
/* }}} */

  // While()と i++ の速度を調べる
  // 没。最適化のせいか、なんなのか、無関係なところを変えたりしただけでも結果が全然変わる。
  // Twhile=0.4666 とか 0.1752 とか全然当てにもならない。
/* {{{ */
  //次にタイマーや割り込みが分からない雑魚なので while() で一定回数繰替えして micros() を
  //読んで速度を調べるので while() とその中の i++ 自体を調べる。
  // t2=10000 Twhile + 10000 Tinc
  // t3= 5000 Twhile + 10000 Tinc
  // よって
  // Twhile = (t2-t3)/5000
  // Tinc = (t2-10000 Twhile)/10000 or (t3-5000 Twhile)/10000
  // 単純に while() の中は i++ だけの予定の予定だったのだが、lto のせいか、最適化のせいか
  // 分からないけど 何回くりかえそうが、3us で終ってしまう。よって micros() も呼ぶことにし
  // た。ループ内の 10000回と外の1回を引けば良い。

  i=0;
  t0=micros();
  while(i<9999){
    t1=micros();
    i++;
  };
  t1=micros();
  t2=t1-t0-10001*Tmicros;

  //Serial.print(", t0=");
  //Serial.print(t0);
  //Serial.print(", t1=");
  //Serial.print(t1);
  //Serial.print(", t2=");
  //Serial.print(t2);

  i=0;
  t0=micros();
  while(i<9999){
    t1=micros();
    i++;
    t1=micros();
    i++;
  }
  t1=micros();
  t3=t1-t0-10001*Tmicros;

  //Serial.print(", t0=");
  //Serial.print(t0);
  //Serial.print(", t1=");
  //Serial.print(t1);
  //Serial.print(", t3=");
  //Serial.print(t3);

  Twhile=float(t2-t3)/5000.0;
  Tinc=(float(t2)-10000.0*Twhile)/10000.0;
  //Tinc=-0.0122 と僅かに負となる。micros() や while() に比べたら、ほぼ0なのだろう
  if(Tinc<0.0){
    Tinc=0.0; 
  };

  Serial.print(", Twhile=");
  Serial.print(Twhile,4);
  Serial.print(", Tinc=");
  Serial.print(Tinc,4);
/* }}} */


  Serial.println();
}

void loop() {
}

