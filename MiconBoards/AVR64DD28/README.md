# はじめに

AVR64DD28 でマイコンボードを作ってみたい。

# 研究

ピン配置等はここ

https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DD28.md

こちらに開発ボードを作って売っている方がいる。

http://ikkei.akiba.coocan.jp/ikkei_Electronics/AVR64DB32.html

で、気になるのが BOOTLOADER 経由の UART に DTR ではなく RTS と記号が振ってある。

自作 UART は RTS の準備がないので、DTR じゃないのと思いつつ本家をチェック。

https://github.com/SpenceKonde/DxCore

すると

> Note the exception about not having autoreset unless you disable UPDI (except for the 20
> and 24-pin 2-Series parts which can put reset on PB4 instead), which is a bit of a
> bummer.

と、なんだかどのマイコンにも当て嵌らないことが書いてある。どうも MegaTinyCore からの派生の
ようで、これはドキュメントの更新が間に合ってないのだろう。

普通に F7ピンが UPDI で F6 が RST で単独で存在してる。

こちらの方は AVR128DB28 で開発ボードを作っているが FT231 の DTR を ATMega328P と同様に 0.1uF
経由で RST ピンに当ててるので、同じで良いのではないだろうか。

https://sunday-engineer.jimdofree.com/2024/08/10/avr128db28%E3%81%A7arduino-3/

DxCore の ADC を調べてみたところ ADC クロックは 2MHz を上限としているようだ。プリスケーラ
が 2 から選べるので 24MHz/2 = 12MHz で動作自体はするはずだが (値を読めるかどうかは知らない
けど)、おそらく最大定格 38.17章の Tclk_ADC 最小の 0.5us を採用したということだと思われる。

プログラマは UPDI. 詳しくはここ

https://github.com/SpenceKonde/AVR-Guidance/blob/master/UPDI/jtag2updi.md

Are you sure? とか言われているが自作 UART に抵抗入れてないから、この決戦パターンで良いのだろう。
参考回路とか普通のダイオード使っているが、シリコンは駄目、ショットキー使えと言っているので守る。

```
No internal resistor on adapter.
Yes resistor on target >= 100 ohms and not more than a few k.

--------------------                                   To Target device
                DTR|                                  __________________
 No resistor?   Rx |--------------,------------------| UPDI----\/\/\/------>
  Are you sure? Tx |----|<|------`          .--------| Gnd     > 100
 This is rare!  Vcc|---------------------------------| Vcc     < 2.2k
                CTS|                     .`          |__________________    Resistor of around a few hundred to a few k
                Gnd|--------------------'
--------------------
```

# 開発ボード 1.0

RST は Datasheet 4.3 によると内部でプルアップしてるとのことなので、プルアップ抵抗は不要。

参考回路の 2個は RST から 5V に向けてのダイオードがないのだが、ATMega328P と同じやりかたで
コンデンサ経由の DTR でのリセットならパルス的な電圧が立つはずなので、入れておくことにする。

スイッチの手前の 330Ωは不要だと思うが Datasheet のおすすめなので、入れておく。手持ちで
330Ω以下の適当なもので良いと思う。内部プルアップ抵抗が不明なので 330Ωは超えない方が良さ
そう。

VDDIO2 は VDD と別電圧を入れられるようだ。3.3V と 5V の信号線の混在とかできるんだと思う。
念のため、ジャンパでシールド側からも供給できるようにしておく。多分使わないけど。

[回路図1.0](./kicad/AVR64DD28_1.0/AVR64DD28_1.0.pdf)

VDDIO2 はスペースの都合で、ただのジャンパ線となった。広げればピンヘッダのジャンパのやつが
使えるけど、必要かどうか分からないので、広げないことにした。

[設計図1.0](./librecad/AVR64DD28_v1.0.pdf)

部品表

| 記号  | 品番、品目               | 個数 |
| ---   | ---                      | ---  |
| B1    | ユニバーサル基板 10x20P  | 1    |
| C1-4  | セラコン 0.1uF 前後      | 4    |
| D1,2  | LED                      | 2    |
| D3    | ダイオード               | 1    |
| J1,2  | ピンソケット 14P         | 2    |
| J3    | ピンソケット 2P          | 1    |
| J4    | ピンソケット 3P          | 1    |
| R1-4  | 1kΩ 前後                | 4    |
| R5    | 330Ωまたは以下          | 1    |
| S1    | IC ソケット 28P          | 1    |
| SW1   | プッシュスイッチ 2P      | 1    |
| U1    | AVR64DD28                | 1    |
| ---   | ---                      | ---  |
| B2    | ユニバーサル基板 7x9P    | 1    |
| D4    | LED                      | 1    |
| H1,2  | ピンヘッダ 7P            | 2    |
| H3    | ピンヘッダ 2P            | 1    |
| H4    | ピンヘッダ 3P            | 1    |
| R6,7  | 1kΩ 前後                | 2    |
| ---   | ---                      | ---  |
| B3    | ユニバーサル基板 7x10P   | 1    |
| D5    | ショットキーなダイオード | 1    |
| D6    | LED                      | 1    |
| H5-7  | ピンヘッダ 7P            | 3    |
| R8    | 300Ω                    | 1    |
| R9,10 | 1kΩ前後                 | 2    |
