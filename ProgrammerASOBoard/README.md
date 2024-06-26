# ASOBoard プログラマの開発

## 考えていること

普通の Arduino IDE からの書き込みに失敗する ATMega328P に当たる確率が高いことを考えると、
手軽にプログラミングできる手段を用意しておきたい。

プログラマの候補としては手持ちでは Arduino Uno R3 (互換含む) と FT232RL (avrdude による)
がある。

Arduino Uno R3 を使う場合、双方にケーブル接続されたシールドを載せて使うことになる。Arduino
は VCC が反対側のソケット、または ICSP ヘッダに D10 が無いし、ASOBoard は RST がD10-13,
GND, VCC の反対となってしまうからだ。Arduino Uno R3 は 2個余ってる (ASOBoard に愛着が涌く
のでそっちを使いたくなるだけで、悪いわけではない) ので、さほど悪いアイデアでは無いと思う。

FT232RL の場合、ASOBoard と継いでいる FT232RL はソケット挿入で取り外し可能にしてある。ただ
FT232RL くらいの幅だと、ソケットからの取り外しがいちいち不安になるくらい硬い。一方配線に使
っている XH コネクタは爪さえ切り落しておけば、まあまあ抜き差しできる。

すでに FT232RL からは VCC, GND, TxD, RxD, DTR# は出ているので、プログラマとして追加が必要
なのは RTS# の一本。なのでもう一本出すようにする。これならばさほど大きくならずに済むだろう。
ASOBoard には専用シールドを作り、プログラマのときだけ、FT232RL からの XH の接続先を変える
ことで対応できそうだ。

一方 ASOBoard 側の機能として、スイッチで RxD, TxD, RTS# の接続先を切り換えるようなことは、
スペース的に難しそうだ。せっかく小さくまとまっている ASOBoard がでかくなってしまう。

面白そうだから両方作って試してしまえば良いだろう。

## FT232RL版 ver 1.0

### FT232RL 側のアダプタ
FT232RL 側のアダプタの回路図はこちら。

[FT232RL アダプタ回路図](./ProgrammerForAsoBoard/kicad/FT232RL_Adaptor/FT232RL_Adaptor.pdf)

設計図はこちら。なるべく現行の延長線上で。

![FT232RL アダプタ設計図 1.0](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_v1.0.png)

配線済み基板切削図はこちら。左右反転済みなので、そのまま彫れば良い。

![FT232RL アダプタv1.0 切削図](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_v1.0_board_reversed.png)

部品表

| 記号 | 品目                   | 個数 |
| ---  | ---                    | ---  |
| D1   | LED                    | 1    |
| J1,2 | 3ピン XH ヘッダ        | 2    |
| J3   | 2ピン XH ヘッダ        | 1    |
| JR1,2| ジャンパー             | 2    |
| R1   | 1kΩ                   | 1    |
| S1,2 | 12ピンソケット         | 2    |
| -    | ユニバーサル基板 15x10 | 1    |

### ASOBoard 側のプログラムシールド

回路図 pdf

[ASOBoard FT232RL プログラムシールド v1.0](./ProgrammerForAsoBoard/kicad/ASOBoardShield_FT232RLProgrammer/ASOBoardShield_FT232RLProgrammer.pdf)

設計図

![FT232ShieldV1.0](./ProgrammerForAsoBoard/librecad/ABSheild_FTProgrammer_v1.0.png)

十字配線基板切削図。反転済み。

![FT232Shiled 切削図 v1.0](./ProgrammerForAsoBoard/librecad/ABSheild_FTProgrammer_Cutting_v1.0.png)

部品表

| 記号 | 品目                   | 個数 |
| ---  | ---                    | ---  |
| H1   | ピンヘッダ、計 11ピン  | 11   |
| J1   | XH 3ピンヘッダ         | 1    |
| J2   | XH 2ピンヘッダ         | 1    |
| UB1  | ユニバーサル基板 15x14穴 | 1  |

### 感想

これで行こうと思って、十字配線基板を彫ってみてしまったんだけど、やっぱり FT232RL 側の配線
が嫌だな。3-3-2 で 7線ではなくて、7列、3-4, 3-2-2, というようにしたほうが収まりが良い。彫
ってしまった十字配線基板はなにかの練習に使うとして、きっちり作りなおした方が、のちのちの自
分が納得するのではないだろうか。

## FT232RL版プログラマ ver 1.1

使用する XH コネクタとそれに関係する設計の違い。方針は

- FT232RL 側は 7ピンコネクタにする
- シールド側は 4 ピンにする。
- ケーブルは電源 3ラインと信号 4ラインに分ける
  - そうなると ASOBoard も変更が必要になる。
  - ASOBoard の配線をシールドの配線よりも優先するため、設計の順番は FT232RL アダプタ,
    ASOBoard, シールドの順番となる

### FT232RL アダプタ ver 1.1

設計図

![FT232RL アダプタ 1.1](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_v1.1.png)

十字配線基板切削パターンはこちら。細線は切削をさぼっても大丈夫なところ

![FT232RL アダプタ 基板パターン 1.1](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_pattern_v1.1.png)

部品表

| 記号 | 品目                   | 個数 | 備考 |
| ---  | ---                    | ---  | ---  |
| D1   | LED                    | 1    |      |
| J1   | 7ピン XH ヘッダ        | 1    |      |
| JR1  | ジャンパー             | 1    |      |
| R1   | 1kΩ                   | 1    |      |
| S1,2 | 12ピンソケット         | 2    | 必要なところだけで OK |
| UB1  | ユニバーサル基板 15x9  | 1    |      |
| X1   | XH 7ピンハウジング     | 1    | 配線用 |
| X2   | XH コンタクト          | 7    | 配線用 |
| X3   | 適当なケーブル         |      | 配線用 |

### ASOBoard シールド ver 1.1

ASOBoard v2.2 で信号線の接続順番が決まったので、こちらも設計しなおす。

https://github.com/Nobutarou/ArduinoDeAsobu/blob/main/OreDuino_wo_Tsukuritai/ASOBorad.md

設計図

![設計図](./ProgrammerForAsoBoard/librecad/Shield_FTProg_v1.1.png)

十字配線基板の切削パターンはこちら

![切削パターン](./ProgrammerForAsoBoard/librecad/Shield_FTProg_v1.1_pattern.png)

部品表

| 記号 | 品目                     | 個数 |
| ---  | ---                      | ---  |
| S1   | ピンヘッダ、計 11ピン    | 11   |
| UB1  | ユニバーサル基板 15x14穴 | 1    |
| XH1  | XH 4ピン ヘッダ          | 1    |

ケーブルは ASOBoard で作ったものを抜いて使うので作成不要。

#### 使い方

![写真](./ProgrammerForAsoBoard/pictures/FT232RLプログラマシールド1.1.jpg)

```zsh
setopt interactivecomments

# ブートローダ書き込み前はヒューズビットを自力で変更する必要がある。出荷状態は、おそらく
# 1MHz 動作でありボーレートは 19200bps が上限だったため ``-B`` オプションが必要。Arduino の
# 値で書き込む

avrdude -v -V -c ft232r -p m328p -B 19200 -U lfuse:w:0xFF:m -U hfuse:w:0xD6:m \
-U efuse:w:0xFD:m -U lock:w:0xCF:m

# 初回はブートローダが無いのでブートローダごと書き込む。たいした時間じゃないから毎回ブートロ
# ーダーごとでも問題ない。例題の Blink の場合は以下。16MHz動作しているのでボーレートの指定は
# 不要。
# -D オプションを付けると動かないので -D オプションは付けないこと

avrdude -v -V -c ft232r -p m328p -Uflash:w:Blink.ino.with_bootloader.hex:i

# ハードに問題がなければ Arduino IDE から次回から書き込みができる
```

## ASOBoard as Programmer v1.0

配線は、とても簡単。

| 親機 | 子機 |
| ---  | ---  |
| VCC  | VCC  |
| GND  | GND  |
| D10  | RST  |
| D11  | D11  |
| D12  | D12  |
| D13  | D13  |

親機の設計図, 部品表

| 記号 | 品目、品番              | 個数 |
| ---  | ------------            | ---  |
| PH1  | ピンヘッダ 8 極         | 1    |
| UB1  | ユニバーサル基板 8x3 穴 | 1    |
| XH1  | XH ヘッダ 6 極          | 1    |

![親機](./ProgrammerForAsoBoard/librecad/ASB_as_Prog_v1.0_parent.png)

子機の設計図, 部品表

![子機](./ProgrammerForAsoBoard/librecad/ASB_as_Prog_v1.0_client.png)

| 記号 | 品目、品番              | 個数 |
| ---  | ------------            | ---  |
| PH1  | ピンヘッダ 8極          | 1    |
| PH2-4| ピンヘッダ 2極          | 3    |
| UB1  | ユニバーサル基板 15x14穴| 1    |
| XH1  | XH ヘッダ 6極           | 1    |

ケーブル部品表

| 品目、品番                  | 個数 |
| ------------                | ---  |
| スリムロボットケーブル 40cm | 1    |
| XH ハウジング 6極           | 2    |
| XH コンタクト               | 12   |

実際に書き込んで見たところ。左が親機で、右が子機。

![書き込んでみたところ](./ProgrammerForAsoBoard/pictures/左から右にLチカ書き込み.jpg)

## ASOBoard as Programmer for Breadboard v1.0

ASOBoard のマイナーアップデートを考えていたところ、そもそもブレッドボードで、

https://x.gd/lBr5Q 

の回路で書き込みができる物が欲しくなってきた。毎回やるたびにどこか間違えるし、かと言って事
前テストをせずに ASOBoard を作ってから書き込むのはトラブル対応に弱すぎると思う。

方針としては

- ブレッドボードには ATMega328P のみ差す
- つまり R 10k, クリスタルと発振用のコンデンサは、こちらが準備する。実際には村田のセラロッ
  クを使う予定。

あと ASOBoard の PIC プログラムシールドを作ったときに、XH コネクタを PIC の真上に来る設計
にしたところ、コネクタのはんだと PIC が干渉して、ちゃんと最後まで差せないという問題が起き
た。

https://x.gd/vJV9x

そこで今回は

- 長くなって良いので ATMega328P の上にはんだが来ないように部品配置する

という 3 方針としてみる。

親機は ASOBoard as Programmer v1.0 と同じ。子機は 5列のブレッドボードに入るように狭くして
10k 抵抗とセラロックを載せれば OK のはず。ケーブルは ASOBoard as Programmer v1.0 から外し
て使おう。もし頻繁に両者を使うことになるようだったら、作ろう。

子機の設計図

![子機設計図](./ProgrammerForAsoBoard/librecad/ASB_as_Prog_for_BredB_1.0-client.png)

子機部品表

| 記号   | 品目、品番                                           | 個数    |
| ---    | ---                                                  | ---     |
| C1     | C 0.1uF                                              | 1       |
| JR1-3  | ジャンパ                                             | 2       |
| PH2    | ピンヘッダ 8P                                        | 1       |
| PH3-5  | ピンヘッダ 2P                                        | 3       |
| PH6    | ピンヘッダ 4P                                        | 1       |
| R1     | R 10kΩ                                              | 1       |
| UB1    | ユニバーサル基板 12x14P                              | 1       |
| X1     | 村田のセラロック 16MHz                               | 1       |
| XH2    | XH ヘッダ 6P                                         | 1       |

完成品。ブートローダと Lチカの書き込みに成功。

![完成図](./ProgrammerForAsoBoard/pictures/ASOBoard_ATMegaProgrammer_on_BredBoard_1.0.jpg)

これでもまだ底付きするが、まあ妥協しておこう。

## ASOBoard as Programmer 親機 v1.1

親機のみの変更。RST-GND 間に 10uF を挿入。MiniCore 使用時にリセットが掛かると avrdude が
urboot ブートローダと握手してしまうので、リセットを掛からないようにする。シミュレーション
してみれば、RST ピンへの電圧が DTR の影響をほとんど受けないことが分かる (いつでもできるの
で割愛)。

![親機 v1.1](./ProgrammerForAsoBoard/librecad/ASB_as_Prog_v1.1.png)

ケーブル、子機は v1.0 のものをそのまま使える。コンデンサは充電時間を待てるなら大きくても良
い。まあ 10uF 台が現実的なところではないだろうか。

部品表 v1.1

|記号    | 品番、品目                                   | 個数    |
| ---    | ---                                          | ---     |
| C1     | 10uF~                                        | 1       |
| PH1    | ピンヘッダ 8P                                | 1       |
| PH2-5  | ピンヘッダ 2P                                | 4       |
| UB1    | ユニバーサル基板 15x14P                      | 1       |
| XH1    | XH ヘッダ 6P                                 |   1     |

子機に Lチカを書き込んだところ

![完成品](./ProgrammerForAsoBoard/pictures/ABProg_prime_v1.1.jpg)
