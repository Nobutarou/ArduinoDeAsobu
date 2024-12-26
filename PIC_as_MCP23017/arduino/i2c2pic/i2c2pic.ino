#include <Wire.h>

// 電源起動時とリセットの時だけのみ処理する関数
void setup()
{
     Serial.begin(9600) ;               // シリアル通信の初期化
     Wire.begin() ;                     // Ｉ２Ｃの初期化、マスターとする
     delay(5000) ;                      // ５秒後に開始
}
// 繰り返し実行されるメインの処理関数
void loop()
{
     int ans , i , j ;
     char c ;

     for (i=0x30 ; i<=0x39 ; i++) {
          Wire.beginTransmission(8) ;   // 通信の開始処理、スレーブのアドレスは８とする
          Wire.write((byte)i) ;         // 通信データを準備する('0'～'9')
          ans = Wire.endTransmission() ;// データの送信と終了処理
          delay(100) ;                  // スレーブがデータを準備するまで待つ
          ans = Wire.requestFrom(8,1) ; // スレーブにデータ送信要求をだし、１バイト受信する
          for (j=0 ; j<ans ; j++) {  // 受信したデータの個数(今回はans=1)だけ繰り返す
               c = Wire.read()  ;       // １バイトデータを読み取る('A'～'I')
               Serial.print(c) ;        // ＩＤＥにシリアル通信で表示する
          }
          delay(1000) ;                 // １秒ウエイト
     }
}
