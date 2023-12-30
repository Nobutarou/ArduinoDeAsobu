# アソボード開発

UNO 互換配置は難しすぎるのであきらめた。

[UNO 互換配置に挑戦はしてみた](./UnoLayoutDesign_giveup.md)

そこで、素直に ATMega328P のピン配置をそのまま使うことにした。

ATMega328P (A) の配置を、そのまま (Sonomama) 使うマイコンボード (Board) ということで、
このリポジトリでは ASOBoard, アソボードとすることにした。というのも、作者が某duino ではなくて、ちゃんと名前を
付けてほしいと考えているそうだからである。

https://trac.switch-science.com/wiki/ArduinoCompatible

## 省スペース化のために FT232RL の EEPROM 書き換え

秋月のマイコンボードでは、FT232RL の CBUS0, CBUS1 から、それぞれ RX, TX 信号に応じた Lチカ
をしている。ただし FT232RL 自身は EEPROM の書き換えにより CBUS の挙動を変えられる。RX, LX
の光り方を自分がなにか判別できるわけではないので、CBUS0 を TX, RX 両方の信号に応じた Lチカ
とすることで、LED を 1個減らす。

Linux には FTDI 謹製ソフトはないので、次を使う。

https://github.com/eswierk/ft232r_prog

古いコードなので libusb-compat と libftdi-compat が必要と思われる。``make`` したら

```
echo "使い方チェック"
./ft232r_prog --help

echo "現在の設定を確認"
./ft232r_prog --dump

echo "CBUS0 の書き換え"
./ft232r_prog --cbus0 TxRxLED
```

で完了。

## 設計方針

- ATMega328P のピン配置をそのまま使うことにしたので、基板も Sunhayato の一番安い小型の
  ICB-288 を、カットとかせず、そのまま使うことにする。
  - https://shop.sunhayato.co.jp/collections/universal-boards/products/icb-288
- リセットボタンは付ける。
- 電源+D13+TxRx の LED 3個は付ける。
  - 3.3V ラインの利用で抵抗を省く (Vf=3.8V)
- ICSP ヘッダは要らない。D10-D13, VCC,(AVREF は挟まるが) GND と一列に並ぶため。
- 余ったスペースはソケットでブレッドボード化する。

## 配線図、図面、部品表

![kicad 配線図](./kicad/ASOBoard/ASOBoard_kicad.png)
[kicad 配線図 PDF](./kicad/ASOBoard/ASOBoard_kicad.pdf)
kicad による配線図

![Librecad 図面](./librecad/ASOBoard_librecad.png)
[LibreCAD 図面 PDF](./librecad/ASOBoard_librecad.pdf)
LibreCAD による図面

| 記号 | 品目                   | 品番                           | メーカー     | 個数 |
| ---- | ----                   | ---                            | --------     | ---- |
| C2,3 | セラコン 22pF          | RD15N220J1HL2L                 | Supertech    | 2    |
| C4-6 | セラコン 0.1uF         |RPEF11H104Z2P1A01B              | Murata       | 3    |
| LE1-3| LED Vf=3.8V            | SLP-WB89A-51                   | 三洋         | 3    |
| MC   | マイコン               | ATmega328P                     | MicroChip    | 1    |
| R2   | 抵抗 10kΩ             | CFS50J10KB                     | Faithful Link| 1    |
| R3,4 | 抵抗 300Ω (注1)       | CF25J300RB                     | Faithful Link| 2    |
| R5   | 抵抗 75Ω              | CFS50J75RB                     | Faithful Link| 1    |
| RJ1-6| ジャンパー抵抗 0Ω     | CFS50J0RB                      | Faithful Link| 6    |
| S1,2 | ソケット 1x14 H3.5     | FSS-41035-14                   | Hirosugi     | 2    |
| S3,4 | ソケット 1x14 H8       | FSS-41085-14                   | Hirosugi     | 2    |
| S5,6 | ソケット 1x12 H3.5     |     FSS-41035-12               | Hirosugi     | 2    |
| S7,8 | ソケット 2x14 H3.5     | FSS-42035-14                   | Hirosugi     | 2    |
| SW1  | タクトスイッチ         | DTS-63-N-V-BLK(TS-0606-F-N-BLK | Cosland      | 1    |
| UB1  | ユニバーサル基板       | ICB-288                        | Sunhayato    | 1    |
| X1   | クリスタル 16 or 20MHz | 不明                           | 不明         | 1    |

注1) 秋月のマイコンボード (https://akizukidenshi.com/catalog/g/gK-04590/) を見ながら 1kΩ
で考えてたけど、良く考えたらブレッドボードで FT232RL と ATmega328P の TX/RX 同士を直接差し
ていたわけで、それなら別に 1kΩという大きさ自体に大した意味はないはずだから、手持ちの 300
Ωで十分だろう。

