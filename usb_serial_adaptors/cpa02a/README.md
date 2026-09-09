# CPA02A による UART アダプタ 1.0

marutu のよる CP2102 のブレイクアウト基板。IC の各ピンを横に出してあるだけで、UART として
使えるようにはできていない。

https://www.marutsu.co.jp/pc/i/10028/

コンデンサはこちらのデータシートの参考回路の通りで良いと思うが、このままだと LDO の入力
REGIN に 5V が入らない。おそらく、コンデンサが回路図に載っているのは、基板上に載せれるよう
になってるからだろう。

https://www.marutsu.co.jp/contents/shop/marutsu/datasheet/cpa02a.pdf

CP2102 自体のデータシートの参考回路 (Fig.5) を見ながらやった方が良さそう。

https://www.silabs.com/documents/public/data-sheets/CP2102-9.pdf

3.3V 動作の IC だが Table 2 の最大絶対定格を見ると I/O は 5.8V までは壊れないようだし
Table 4 の電気特性に Input High Voltage の Max が何も書いてないことから 5V 信号 OK なので
はないだろうか。

この辺は壊す気持でやってみて覚えよう。

[回路図](./v1.0/cpa02a_1.0/cpa02a_1.0.pdf)

[設計図](./v1.0/cpa02a_1.0.pdf)

必要なピンだけ出すということも考えたが、実際に設計してみると、必要なピンだけ出すことで小型
化できるわけでもないので (自分の実力では)、そのまま D+, D- の USB ラインと REGIN (3.3V LDO
の 5V 入力) 以外はそのまま出すことにした。

部品表

| 記号 | 品番、品目               | 個数 |
| ---  | ---                      | ---  |
| B1   | ユニバーサル基板 8x16P   | 1 |
| C2,3 | セラコン 0.22uF          | 2 |
| J1   | USB-B コネクタ           | 1 |
| J2,3 | ピンソケット 7P          | 2 |
| R1   | 10kΩ                    | 1 |
| S1,2 | 背の低いピンソケット 10P | 1 |
| U1   | CP02A                    | 1 |

## RL78 書き込み器 1.0

TX, RX, TOOL0 のところをこちらでシミュレーションしてみた。

https://tinyurl.com/2cvc8b4l

気になったのは TX への電圧と電流。送信時は 1mA にもならないのできっと OK. RX 受信時、
TX=3.3 で TOOL0=5V だと 4mA くらい流れる。まあ大丈夫じゃね？3.3V とか 3.6V のツェナーを入
れても良さそうだけど、駄目なら駄目で壊して覚えよう。

[設計図](./v1.0/cpa02a_rl78prog_1.0.pdf)

部品表 

| 記号 | 品番、品目              | 個数 |
| ---  | ---                     | --- |
| B1   | ユニバーサル基板 8x8P   | 1 |
| D1   | ショットキーダイオード  | 1 |
| R1   | 10kΩ                   | 1 |
| R2   | 300Ω                   | 1 |
| S1-5 | ピンソケット 1P         | 5 |
| S6-7 | ピンソケット 2P         | 2 |      


ボーレート 115200, 500000bps では書き込みできた。``-s`` でボーレートの指定が必要。自動では失敗する。

```sh
> rfp-cli -d RL78/G2x -port /dev/ttyUSB0 -dtr-inv -a ./LuxMeter1.0.srec -s 500000
Renesas Flash Programmer CLI V1.13
Module Version: V3.20.00.000
Load: "/home/snob/Documents/e2studio/LuxMeter1.0/Release/LuxMeter1.0.srec" (Size=19968, CRC=12DC003F)

Connecting the tool (COM port)
Tool: /dev/ttyUSB0
Interface: 1 wire UART

Connecting the target device
Speed: 500,000 bps
Connected to R7F101G6E

Erasing the target device
  [Code Flash 1]       00000000 - 0000FFFF
  [Data Flash 1]       000F1000 - 000F1FFF
  [Code Flash 1]       00000000 - 000027FF
Writing data to the target device
  [Code Flash 1]       00000000 - 00001BFF
  [Code Flash 1]       00002000 - 000021FF
Verifying data on the target device
  [Code Flash 1]       00000000 - 00001BFF
  [Code Flash 1]       00002000 - 000021FF


Disconnecting the tool
```
