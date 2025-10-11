# v1.0

AVR64DD28, PIC18F27Q43 と ADC 12bit だったので、今さら 10ビットには戻れない。
12bit ADC を持つルネサスマイコンは RL78/G23, G24 シリーズか、RA など他のシリーズ。

これまでが 28ピンマイコンだったけど、ルネサスマイコンは SSOP で 20ピンのものと 30ピンのも
のがあり、SQP だと 32 ピンのものがある。これまでの 28ピンマイコンで一度も 20ピンも使ったこ
とがないので 20ピンで良いだろう。


ということで、12ビット ADC と SSOP 20ピンというところから本機を選んだ。

RL78 シリーズはシリアル通信でプログラミングができるらしく、
Renesas Flash Programmer 対応マイコン表

https://www.renesas.com/ja/document/mat/list-mcus-supported-renesas-flash-programmer-v3?r=488871)

でも可となっている。1線式のタイプB と 2線式のタイプD が対応となるらしい。

1線式は

https://x.gd/IrZhp

の 4.2 節になる。TX にスリーステートバッファが入っている。入力に ○が付いているのは反転？

```
OE --+
 A --+--- Y
```

| OE (TX) | A (GND) | 出力 Y (TOOL0) |
| ---     | ---    | ---    |
| L | H | H |
| L | L | L |
| H | any | Z |

TOOL0 が入力状態 (Z状態) のとき、

- TX = L なら TOOL0 にも L が届く
- TX = H なら Z、つまり断線。RX はいつでも Z なはずだからプルアップされた TOOL0 には H が
  届く。

TOOL0 が送信状態 H のとき、TX の状態に関係なくプルアップされている RX には H が届く

TOOL0 が送信 L のとき、TX は L または断線 Z。つまり RX には L が届く。

これをスリーステートバッファじゃなくてダイオードを TX に向けて入れている方がいる。

https://x.gd/JyZcD

これもしかしてシリアル UPDI も同じ考えだったのでは？

念のため Falstad のシミュレータでダイオードとトリステートバッファの比較

https://tinyurl.com/25pmutjl

| TXD | TOOL0 | ダイオード RX, TOOL0 | バッファ RX, TOOL0 | コメント |
| --- | --- | --- | --- | --- |
| Z | Z | L | H | 両方入力な変なモードだから無効？ | 
| Z | L | L | L | |
| Z | H | H | L | TX は Z にならないから無視できる？|
| L | Z | L | L | |
| L | L | L | L | 両方出力な変なモード |
| L | H | L | L | 両方出力な変なモード |
| H | Z | H | H | |
| H | L | L | L | 両方出力な変なモード |
| H | H | H | H | 両方出力な変なモード |

TXD が Z の時は動作が異なっているが、参考 URL ではそのモードには触れていない。
TXD は出力という役目的に Z にはならないから気にしないと言うことかな。
というかシミュレーターでワイヤーを一本引いたら 0V 扱いになった。バッファの #OE を絶縁した
ときに 0V を入力というふうにシミュレータ上では扱われたので、答が異なっただけだ。
適当に トリステートバッファのデータシート見ても、そもそも入力は High か Low だけで Z はそ
もそも考えない。

ということで、その方が成功しているわけだし、多分 UPDI と同じような仕組みだし、それで良いの
だろう。

RTS にインバータを噛ましているが、Frash Programmer のオプションにリセット信号を DTR, RTS,
反転、そのまま、の 4パターンから選択できるオプションがあるから DTR でも良いはず。

でここには タイプA,B,C しか記載がないが、おそらく タイプD は、このデバグ配線と思われる。D
は Debug ではないだろうか。

https://www.renesas.com/ja/document/apn/rl78-debugging-functions-using-serial-port-application-note

基本、これの図2.1 で良いと思う。図2.4 に Arduino でシリアルモニタを使うときの設定がある。
ぱっと見、プログラミングと UART のどちらでも使えそうに見えるが、TOOLRxD/TxD はあくまでプロ
グラミングやデバグモードでの通信であり、普通の UART はピンが別にある。

なので、これは Arduino を使っているときだけの仕組みだと思うので、図2.1 で良いだろう。

さて結局のところ、(腕的に)プログラマと UART は別ものとして使うことになりそうなので
(PIC18F27Q43 の時同様)、配線の簡素な 1線式で良いだろう。

UPDI のときはパリティ未対応の UART アダプタは使えなかったが、

https://www.renesas.com/ja/document/apn/rl78-flash-programmer-rl78-protocol-c-rev100

でパリティで検索すると、なし、と出てくるのでパリティの無い PIC16F1455 でも書き込める可能性
が出てきた。
(ていうか、USB 機能付きのやつであれば、UART アダプタ作れるんだろうな。ていうか E2 Lite と
か、中身ルネサスのマイコンなんだろうなあ。

なんかルネサスのマイコンって、個人が Lチカしてみた、みたいな記事はほとんどないけど、オフィ
シャルなドキュメントは、どれ読めば分からないだけで、豊富なんだな。シリアル通信でプログラミ
ングできるとか、個人趣味向け？いや DIP 版が無いからそれはないか。)

とハード面は大丈夫かなと思い e2 Stduio で Lチカを書いてビルドしても elf しかできない。hex
ができないと思ったが srec ファイルが hex と同種 (Motolora の hex フォーマット？) らしい。

ということでソフトもなんとかなるかもしれない。

ということで雑だけど、
[回路図](./v1.0/R7F101G6E2DSP_1.0/R7F101G6E2DSP_1.0.pdf)

ストロベリーLinux の FT232RL アダプタは、PIC16F1455 が駄目だった場合のバックアップ。
(そもそも何か勘違いしていて駄目かもしれないけど)
[設計図](./v1.0/R7F101G6E2DSP_1.0.pdf)

製作中に気がついたけど、本体の左上角のランドが浮いてしまってる。たいしたことではないけど浮
きランドを作らないつもりで設計してるので、失敗ポイント。

本体部品表

| 記号 | 品番、品目 | 個数 |
| --- | --- | --- |
| B1 | ユニバーサル基板 11x12P | 1 |
| C1 | セラコン 1.5uF | 1 |
| C2 | セラコン 0.22uF | 1 |
| J1,2 | ピンソケット 2P | 2 |
| J3,4 | ピンソケット 10P | 2 |
| S1,2 | 低いピンソケット 10P | 2 |
| U1 | DIP化した R7F101G6E2DSP | 1|

PIC16F1455 UART 用プログラマアダプタ

| 記号 | 品番、品目 | 個数 |
| --- | --- | --- |
| B2 | ユニバーサル基板 7x9P | 1 |
| D1 | ショットキーダイオード | 1 |
| D2 | LED | 1 |
| H1,2 | ピンヘッダ 1P | 2 |
| H3,4 | ピンヘッダ 3P | 2 |
| H5,6 | ピンヘッダ 2P | 2 |
| R1 | 10kΩ | 1 |
| R2 | 300Ω | 1 |
| R3,4 | LED 用の適当な抵抗 | 2 |

ストロベリー Linux FT232RL 用プログラマアダプタ

| 記号 | 品番、品目 | 個数 |
| --- | --- | --- |
| B3 | ユニバーサル基板 6x4P | 1 |
| D1a | ショットキーダイオード | 1 |
| H7,8 | ピンヘッダ 1P | 2 |
| H9 | ピンヘッダ 5P | 1 |
| H10,11 | ピンヘッダ 2P | 2 |
| R1a | 10kΩ | 1 |
| R2a | 300Ω | 1 |

UART に PIC16F1455 を使っての書き込みテスト。

```
> ./rfp-cli -d RL78/G2x -port /dev/ttyACM0 -dtr -a ./Blink.srec
Renesas Flash Programmer CLI V1.13
Module Version: V3.20.00.000
Load: "/home/snob/.local/share/renesas/rfp/linux-x64/Blink.srec" (Size=7346, CRC=8AFFE60B)

Connecting the tool (COM port)
Tool: /dev/ttyACM0
Interface: 1 wire UART

Connecting the target device

[Error] E3000105: The device is not responding.
The target device is not connected or has not returned a response. Confirm that the connection to the target device and the operating mode are correct. The security functions of the target device may have prevented the connection.
Refer to https://www.renesas.com/rfp-error-guide#no-response, Problems during Operation.

Disconnecting the tool
```

失敗する。DTR ラインをオシロで見ていると、LOW にへばりつく。これでは、マイコンが再起動できないと思う。
-dtr-inv に変えると一瞬 Low で High なるというのを確認。たぶんこちらかな？ただ同じエラーは起きている。

FT232RL だと行けた。やはりシリアルUPDI と同じでパリティが必要なのか。

```
> ./rfp-cli -d RL78/G2x -port /dev/ttyUSB0 -dtr-inv -a ./Blink.srec
Renesas Flash Programmer CLI V1.13
Module Version: V3.20.00.000
Load: "/home/snob/.local/share/renesas/rfp/linux-x64/Blink.srec" (Size=7346, CRC=8AFFE60B)

Connecting the tool (COM port)
Tool: /dev/ttyUSB0
Interface: 1 wire UART

Connecting the target device
Speed: 1,000,000 bps
Connected to R7F101G6E

Erasing the target device
  [Code Flash 1]       00000000 - 0000FFFF
  [Data Flash 1]       000F1000 - 000F1FFF
  [Code Flash 1]       00000000 - 00000FFF
  [Code Flash 1]       00002000 - 000027FF
Writing data to the target device
  [Code Flash 1]       00000000 - 000009FF
  [Code Flash 1]       00002000 - 000021FF
Verifying data on the target device
  [Code Flash 1]       00000000 - 000009FF
  [Code Flash 1]       00002000 - 000021FF


Disconnecting the tool
```

L チカも成功。ただ後から気がついたけど RESET は内部プルアップされていないらしく、自力でプ
ルアップしないといけない。書込み器に繋いでいるときは、#DTR が通常は High で Reset をプルア
ップするんだけど、電源だけを投入したときなどを考えると、プルアップ抵抗を入れておく必要があ
る。

Lチカ、UART 送信、I2C による MCP23017 操作まで成功。LCD も動かせるはずだけど LCD はぶっつ
け本番で良いだろう。

# v1.1
まだ作らないけど v1.0 からの変更すべきとこだけ書いておく。

- RESET をプルアップ
- FT232RL だけ考えて PIC16F1455 は OBSO (普通に UART として使う)
