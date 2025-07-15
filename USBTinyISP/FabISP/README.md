# FabISP を作ってみる v1.0

本家:

https://fab.cba.mit.edu/content/archive/projects/fabisp/

ATTiny85 で作った LittleWire が、USB 接続での認識がいまいちだったので、USB 機器作成用途で
すらないマイコンの適当な水晶ではなく、外部水晶を使った本機を作ってみる。

本機は ATTiny44 を使う。回路図の ATTiny44 が DIP 版のピン配置になっていないので、回路図を
書き直してみる。

ASOBoard, USBasp (ASOBoard と同じにした) 、LittleWire (USBasp と同じにした) と同じレイアウ
トにすることを踏まえた回路図とした。

[回路図](./kicad/FabISP_DIY_1.0/FabISP_DIY_1.0.pdf)

本家との違い:

コンデンサは付けすぎても突入電流が大きいという弊害もあると聞いたので C3 は理由も無さそうな
ので付けない。USB 向け IC みたいに 3.3V LDO とかがあるわけでもないので、そういう大きなコン
デンサは不要だろう。

D- のプルアップは 1.5kΩと規格にあるので、LittleWire では 1kΩ、USBasp では 2kΩとなんとな
く付けて動いていたが、規格を知ったからには守ることにして 1.5kΩとする。絶対私より詳しい人
が決めてるんだから、従おう。

ツェナーの D1, D2 は LittleWire, USBasp では 3.6V で動いているので、これは入手性でどっちか
にしよう。

ついでに R1, R2 は手持ちの 75Ω。USBasp でも LittleWire でも動いている。

6ピンのヘッダは使わずに、自作アタッチメント使うから不要。

SJ1 のところだけど、外から自由にリセット掛けるのを禁止する必要もないので、常時導通。USBasp
と同じ。LittleWire はジャンパ切り替えにしたけど、あれは、自分に書き込むときに RST ピンとし
て使い、プログラマでは SS として使うためだから、今回のには、する必要がない。

C2, C4 は手持ちの 15pF

配置はなるべく GND で埋めて浮きホールを作らないこと (ノイズ対策のつもり) と、なるべく端に
は 5V ラインが来ないよういした (ショート事故対策)

[設計図](./librecad/FabISPDIY1.0.pdf)

部品表:

| 記号  | 品目、品番                     | 個数 |
|-------|--------------------------------|------|
| B1    | ユニバーサル基板 14x19P        | 1    |
| C1    | セラコン 0.1uF 程度            | 1    |
| C2,4  | セラコン 15pF 程度             | 2    |
| D1,2  | 3.3 or 3.6V ツェナーダイオード | 2    |
| J1    | USB B コネクタ                 | 1    |
| Q1    | 12MHz 水晶                     | 1    |
| R1,2  | 75Ω程度                        | 2    |
| R3    | 1.5kΩ                          | 1    |
| R10   | 10k~100kΩ                      | 1    |
| S1-S3 | ピンソケット 2P                | 3    |
| S4    | ピンソケット 8P                | 1    |
| S5    | IC ソケット 14P                | 1    |
| U1    | ATTiny44                       | 1    |

書き込みは同ページの firmware.zip を展開して、Makefile のここを手持ちの ISP プログラマに変
更して、

```sh
#AVRDUDE = avrdude -c usbtiny -p $(DEVICE) # edit this line for your programmer
AVRDUDE = avrdude -c usbasp -p $(DEVICE) # edit this line for your programmer
```

```sh
make program
```

で良いはず。ビルド済みの main.hex が同梱されているので gcc-avr を入れてビルドする必要は無
い。


