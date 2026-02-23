# 01
<!-- {{{ -->
QFN24 のチップから、このやり方でやってみる。

https://www.nicovideo.jp/watch/sm22265444

実はサンハヤトのストレートパターンの基板に付けてから、そこに銅線を付けようと思った。これは
動画のコネクタのところのやりかた。ただ、基板との奥行の位置合せが絶望的に難しくて、深いと
GND パッドまでひっついてしまうし、浅いとぜんぜんはんだできないから、あきらめて、動画の QFN
裏返し法をなんとか物にしてみたいと思う。

基本的には 24 ピンを左右に 12 ピン出す形にしながら、USB-B を付けるイメージ。DIP 化に挑戦し
てみて、最後までうまくできたら、そこに USB-B を付けてしまおうという感じだ。

状態としては、USB に繋いだだけで動作する状態にはする。なのでソケットに各ピンを出しつつも、
必要な電源ラインは基板上で繋いでおく。

[回路図](./v1.0/cp2104_1.0/cp2104_1.0.pdf)

このやり方で成功したことはないので、CP2104 への配線パターンをなるべく変に交差とかしないよ
うに素直な配線とした。
[設計図](./v1.0/cp2104_1.0.pdf)

部品表:
| 記号 | 品目、品番 | 個数 |
| --- | --- | --- |
| B1 | ユニバーサル基板 8x16P | 1 |
| C1 | セラコン 0.22uF | 1 |
| C2 | セラコン 1.5uF | 1 |
| J1 | USB-B コネクタ | 1 |
| J2-3 | ピンソケット 12P | 2 |
| R1 | 10kΩ | 1 |
| U1 | CP2104 | 1 |

[完成品](./v1.0/cp2104_1.0.jpg)
少なくとも RX, TX, DTR は動作。

ピンと銅線の 2D 的な位置合せは、さほど困難ではないので、高さ方向の位置合わせが肝だと思う。
<!-- }}} -->

# 02

今度は必要最小限、RX, TX, DTR だけ出してみる。良くある USB シリアルアダプタみたいに。

[回路図](./v2.0/cp2104_v2.0/cp2104_v2.0.pdf)

[設計図](./v2.0/cp2104_2.0.pdf)

部品表:
| 記号  | 品目、品番                 | 個数 |
|-------|----------------------------|------|
| B1    | ユニバーサル基板 8x10P     | 1    |
| C1    | セラコン 0.22uF            | 1    |
| C2    | セラコン 1.5uF             | 1    |
| J1    | USB-B コネクタ             | 1    |
| J2-3  | ピンソケット 4P            | 2    |
| R1    | 10kΩ                      | 1    |
| U1    | CP2104                     | 1    |

[完成品](v2.0/cp2104_2.0.jpg)

小さくて良い感じ。


とりあえず RF78/GL24 のプログラマも作ってみた。

[設計図](v2.0/rl78_prog.pdf)

3.3V の本機への保護は 300Ωの抵抗とショットキーのみ。ただし Falstad でシミュレーションする
と、まあ壊れないだろうくらいにはなっている。

https://is.gd/816afV

フルの 1Mbps でも 500kbps でもどっちでも成功した。

```
> rfp-cli -d RL78/G2x -port /dev/ttyUSB0 -dtr-inv -a ./DiodeTester3.0.srec
Renesas Flash Programmer CLI V1.13
Module Version: V3.20.00.000
Load: "/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx/DiodeTester3.0.srec" (Size=20508, CRC=FEC0A4D7)

Connecting the tool (COM port)
Tool: /dev/ttyUSB0
Interface: 1 wire UART

Connecting the target device
Speed: 1,000,000 bps
Connected to R7F101G6E

Erasing the target device
  [Code Flash 1]       00000000 - 0000FFFF
  [Data Flash 1]       000F1000 - 000F1FFF
  [Code Flash 1]       00000000 - 000027FF
Writing data to the target device
  [Code Flash 1]       00000000 - 00001CFF
  [Code Flash 1]       00002000 - 000021FF
Verifying data on the target device
  [Code Flash 1]       00000000 - 00001CFF
  [Code Flash 1]       00002000 - 000021FF


Disconnecting the tool
```

```
> rfp-cli -d RL78/G2x -port /dev/ttyUSB0 -dtr-inv -a ./DiodeTester3.0.srec  -s 500000
Renesas Flash Programmer CLI V1.13
Module Version: V3.20.00.000
Load: "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx/DiodeTester3.0.srec" (Size=20508, CRC=FEC0A4D7)

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
  [Code Flash 1]       00000000 - 00001CFF
  [Code Flash 1]       00002000 - 000021FF
Verifying data on the target device
  [Code Flash 1]       00000000 - 00001CFF
  [Code Flash 1]       00002000 - 000021FF


Disconnecting the tool
```
