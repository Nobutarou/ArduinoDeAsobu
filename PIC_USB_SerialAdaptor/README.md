# PIC で USB シリアルアダプタを作る

こういったプロダクトがある。前者はソースがいまいちはっきりしないが後者はソースがはっきりし
ているし .hex も配布されている。

https://elchika.com/article/a4b16182-6270-493c-b4da-7c519ac3429c

https://github.com/Calcousin55/PIC16F145x_USB2Serial

## 検討

### 前者のやつ MPLAB IDE でビルドできるのかの確認

MCC は Classic を使うと同じ画面が出た。また USB_device_CDC.c の 117行目の書き換えが必要と
書いてあるが、似ている部分がない。ただし 117行目というのがちょうど関数定義の始まる部分であ
ることと、その 3行に出ている文字列で ```grap -r``` しても ```#define``` されている気配が
ないことと、```if #defined``` で使われていることから、自力で ```#define``` すれば良いのか
と言う気がしたので 117行目あたりに自力で書いてみた。

```C
#if defined(USB_CDC_SET_LINE_CODING_HANDLER)
CTRL_TRF_RETURN USB_CDC_SET_LINE_CODING_HANDLER(CTRL_TRF_PARAMS);
#endif

/** ここから
#define USB_CDC_SUPPORT_DTR_SIGNALING
#define UART_DTR PORTCbits.RC3
#define USB_CDC_DTR_ACTIVE_LEVEL 0
/** ここまで

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void USBCDCSetLineCoding(void);
```

ビルドは通った。なのであとはブレッドボードで試してみるしかない。

### 後者でテスト 

.hex も配布されているが PIC16F1454 向けにビルドされたようなので PIC16F1455 向けに設定を変
えてビルドしてみたところ普通にビルドできた。

a-p-prog で書き込んで USB 接続してみたところ /dev/ttyACM0 として認識した。

CuteCom で接続して適当に文字を送り LED Tx の挙動を調べた。LED Tx - R - LED - GND の接続で
入力時に光るので FT232RL と同様に GND に引き込む形で光らせる物らしい。

LED Tx, LED Rx に直接テスターを当てても 0V と出るのでそのあたりは意味が分かっていない。

そのまま Arduino IDE から ASOBoard に書き込んでみたところ、普通に書き込めた。

![Lチカ](./figures/PIC_Serial_to_ASOBoard.jpg)

ATMega で書き込んだ PIC で ATMega を書き込むというのは、なかなかロマン度が高い気がした。

ここまで簡単に事が進んだので後者だけ検討していけば良いだろう。

さて LED ピンを共通化したいので io_mapping.h を書き換える。

```diff
--- io_mapping.h.org    2024-03-21 20:25:52.231722197 +0900
+++ io_mapping.h        2024-03-21 20:28:12.195059668 +0900
@@ -153,7 +153,7 @@
     #define LED_Rx_PIN   0

     #define LED_Tx_PORT  C
-    #define LED_Tx_PIN   1
+    #define LED_Tx_PIN   0

     /** I/O pin definitions **************************************************************************/

```

これで LED Rx = LED Tx = RC0 となったはず。ちなみにピン配置図には描かれていないが RA5 に
RTS, RA4 に CTS, RC2 に DSR が割り当てられているらしい。なので分かる人であれば ICSP だかで
プログラマが書けるのだろう。私はできないので、当面はスルー。

いろいろ触っている内に LED が光らなくなってしまった。元に戻しても光らない。壊してしまった
のだろう。ま TX, RX LED は無しにしよう。

実装だけど前に USB を持ってきて、ピンは横にソケットを出して、その上に基板を重ねて必要な線
を出すのが良さそう。無理に FT232RL 用のケーブルを使えるようにせず、素直に出し安い様に出せ
ば良いだろう。

## 実装 ver 1.0

1段目に USB と PIC で 2段目に XH コネクタという設計にしてみた。

![1段目](./librecad/PIC_Serial_v1.0_bottom.png)

![2段目](./librecad/PIC_Serial_v1.0_top.png)


