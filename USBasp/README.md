# USBasp の自作

本家

https://www.fischl.de/usbasp/

自作してる人はたくさんいて、その中で参考にしたのがこちら。マイコンのシンボルが実機と同じ配
置になってて、見易いから。

http://www.cs.shinshu-u.ac.jp/~haeiwa/m-com/USBasp/USBasp_new.html

それから、この方。非常に簡略化されていて、必要な部分が分かりやすい。

https://deltav-lab.techblog.jp/archives/30113269.html

最初に思った感想が、なんで USB 機能のあるマイコンを使わないんだろうってところ。

## ver 1.0

まず分からないのが D-, D+ に配置されている定電圧ダイオード。

https://www.falstad.com/circuit/circuitjs.html

で計算しても、電圧は 3.3V か 0V で信号レベル変換にはなっていない。
[回路ファイル](./misc/USB-D_Diode_why.txt)

![D が High](./misc/USB-D_High.png)

![D が Low](./misc/USB-D_Low.png)

寄生インダクタンスによる電圧のオーバーシュートが恐いとしても、信号ラインにダイオード入れて
るのは初めての経験。なんとなくだけど 5V マイコンのために昇圧しようと考えて入れたけど、実は
働いていないということなのではないだろうか。

その先の抵抗も分からない。

そもそも Arduino Uno R3 の Tx, Rx に何故抵抗が入っているのかも良く分からない。他から Tx,
Rx 使われたときの保護とある。まあ一応他が High で自分が Low のとき、理想的な回路なら無限の
電流が流れるから、それに対する保護かな。ただマイコンのピンは対して電流流せないから、そんな
ことしても、High 側の電圧が下がるだけだとは思うけど。

https://forum.arduino.cc/t/resistors-on-the-rx-and-tx-lines-of-the-arduino-pro/154783/4

少なくとも、本機で、2個以上の信号線が混ざることはないから、保護目的なる抵抗は不要だ。

RESET をヘッダに出しているのは、本機をプログラムするときのためのもののようだ。本機はブレボ
で書き込むから、これも不要だ。

そんなこんなで
[回路図1.0](./kicad/USBasp_1.0/USBasp_1.0.pdf)

まずはブレッドボードで組んで ASOBoard に Lチカでも書き込んでみるところから始めよう。設計は
後回し。LED は要らん。必要になっても手持ちの LED と抵抗でなんとかなる。ジャンパもブレボ上
ならなんとでもなるので不要。

試験向け部品表

| 回路図記号 | 品目等 | 個数 |
| ---        | ---    | ---  |
| C1         | ケミコン 1uF | 1 |
| C2         | セラコン 0.1uF   | 1 |
| C3,4       | セラコン 15pF    | 2 |
| J1         | USB B Dip 化基板 | 1 |
| R1         | 10kΩ            | 1 |
| R4         | 2kΩ             | 1 |
| U1         | ATmega8-16PU     | 1 |
| Y1         | 水晶 12MHz       | 1 |

USB デバイスとして認識するところまではできたが、どうやっても接続相手を認識しない。実力不足
でギブアップ。

[設計図](./librecad/USBasp1.0.pdf)。一応何か勘違いして回路を間違えていた可能性があるので、
作ってみる。ASBoard 用 Arduino as ISP シールドがそのまま挿せるようにしてある。これで駄目な
ら、大人しく、Arduino as ISP 一本でなんとかしていくことにする。

実装用部品表

| 設計図記号 | 品目等                  | 個数 |
| ---        | ---                     | ---  |
| C1*        | ケミコン 1uF            | 1    |
| C2         | セラコン 0.1uF          | 1    |
| C3,4       | セラコン 15pF           | 2    |
| D1,2*      | LED                     | 2    |
| IS1        | IC ソケット 28P         | 1    |
| J1         | Omron USB-B コネクタ    | 1    |
| JR1-3      | ジャンパ線              | 3    |
| PH1        | ピンヘッダ 2P           | 1    |
| PS1,2      | ピンソケット 14P        | 2    |
| R1         | 10kΩ                   | 1    |
| R2,3*      | 1kΩ                    | 2    |
| R4         | 2kΩ                    | 1    |
| U1         | ATmega8P                | 1    |
| UB1        | ユニバーサル基板 21x14P | 1    |
| Y1         | 水晶 12MHz              | 1    |

*) は任意

結果は、あっさり動き ATmega8P に対して Minicore ブートローダと Lチカの書き込みに成功。原因
は分からない。

ただし制作から試行までで、一カ所設計間違いを見つけた。D- のプルアップを間違って D+ に行な
っていた。設計図と回路図と実物を見比べながら確認しやすい一方で、ブレッドボードのときは設計
図なしでいきなり配線するので、間違いに気がつきにくい。5V と GND のショートも何度もやってた
りする。というようなことを考えるとこの手のマイコン系のテストは、最初から実装してみても良い
のかもしれない。ブレッドボードで動かなくても、結局実装して確認するのであれば、やらなくて良
さそう。

あと Udev のルールが付属の物だと動かなかったので、こうしてある。

```
# USBasp - USB programmer for Atmel AVR controllers
# Copy this file to /etc/udev/rules.d so

#SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", SYSFS{idVendor}=="16c0", SYSFS{idProduct}=="05dc", MODE="0666"
SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", ENV{ID_VENDOR_ID}=="16c0", ENV{ID_MODEL_ID}=="05dc", MODE="0666"

#ID_BUS=usb
#ID_MODEL=USBasp
#ID_MODEL_ENC=USBasp
#ID_MODEL_ID=05dc
#ID_SERIAL=www.fischl.de_USBasp
#ID_VENDOR=www.fischl.de
#ID_VENDOR_ENC=www.fischl.de
#ID_VENDOR_ID=16c0
#ID_REVISION=0104
#ID_USB_MODEL=USBasp
#ID_USB_MODEL_ENC=USBasp
#ID_USB_MODEL_ID=05dc
#ID_USB_SERIAL=www.fischl.de_USBasp
#ID_USB_VENDOR=www.fischl.de
#ID_USB_VENDOR_ENC=www.fischl.de
#ID_USB_VENDOR_ID=16c0
#ID_USB_REVISION=0104
```

## ver 1.1

D+, D- の定電圧ダイオードが ATmega8 側の 5V を USB 側に伝えないためだと分かった。

https://tinyurl.com/29rq6v27

ATmega8 側が 5V, USB 側が入力 (コンデンサ) となっているときに何もしないと 5V 入ってしまう。

ということで追加する。

[回路図1.1](./kicad/USBasp_1.1/USBasp_1.1.pdf)

アノテーションを振り直したので記号に一貫性がないけど 1.0 から R1,3 = 75Ω と D1, D2 3.6V
ツェナーダイオードを追加している。

設計は完成している v1.0 の改造とする。

[設計図1.1](./librecad/USBasp1.1.pdf)

信号線に 4箇所切れ目を入れるだけで良い。最後に残ったところは全部 GND にしてるので、掘る必
要はない。(十字配線済み基板を利用)

部品表

| 設計図記号 | 品目等                  | 個数 |
| ---        | ---                     | ---  |
| C1*        | ケミコン 1uF            | 1    |
| C2         | セラコン 0.1uF          | 1    |
| C3,4       | セラコン 15pF           | 2    |
| D1,2**     | ツェナーダイオード 3.6V | 2    |
| D3,4*      | LED                     | 2    |
| IS1        | IC ソケット 28P         | 1    |
| J1         | Omron USB-B コネクタ    | 1    |
| JR1-3      | ジャンパ線              | 3    |
| PH1        | ピンヘッダ 2P           | 1    |
| PS1,2      | ピンソケット 14P        | 2    |
| R1,3**     | 75Ω                    | 2    |
| R2         | 2kΩ                    | 1    |
| R4         | 10kΩ                   | 1    |
| R5,6*      | 1kΩ                    | 2    |
| U1         | ATmega8P                | 1    |
| UB1        | ユニバーサル基板 21x14P | 1    |
| Y1         | 水晶 12MHz              | 1    |

＊＊) v1.0 に対して追加する部品。
