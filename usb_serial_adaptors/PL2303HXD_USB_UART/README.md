# v1.0

うーん、入手が難しいけど Prolific は SSOP16 の PL2303GS っていう新しいものも出してるんだな
あ。なんか、せっかくだからそっちが欲しくなってきたぞ。

この IC は 3.3V なので、RXD は USBtinyISP などの USB ラインでやったときのように、直列の抵
抗とツェナーを入れる。300Ωとしたので流れる電流は (5V-3.3V)/300Ω = 5.7mA で良いだろう。

また 5V を直接挿すという実験もやってみたいところ。実は USBasp のときにツェナーの意味を分か
ってなくて直結をしているが、そのときは壊れたり、異常動作というのはしなかった。そもそも理想
的には入力端子には電流は流れない。

I/O ピンの基本的な構造はこちららしい。

https://edn.itmedia.co.jp/edn/articles/1712/27/news011_2.html

これを試してみたが、1kΩを直列に入れておくと、マイコン側では 3.8V 弱に落ちる。ということで、
1kΩ入れておけば十分なんじゃないかと言う気もする。

https://tinyurl.com/26eotjp2

Arduino の RX,TX ラインに入ってる 1kΩも 5V を越える入力を入れてしまった時の為用だかなんだ
というのをどこかで見たきもするし。

まあこれは別バージョンでの試験としよう。

せっかくなので RL78 1線式 UART プログラマアダプタも検討する。

Falstad で練習。https://tinyurl.com/23yascmb

判定は [FT232RL のとき](https://tinyurl.com/25pmutjl) と比べて High Low が違ってないかどうか。

| TX | TOOL | RX | 判定 |
| --- | --- | --- | --- |
| 3.3V | 5V | 3.3V | OK |
| 3.3V | 0V | 0.1V | OK |
| 3.3V | Z, 3.3V | 3.2V | OK |
| 0V* | 5V | 0.6V | OK |
| 0V* | 0V | 0.1V | OK |
| 0V | Z, 0.4V | 0.4V | OK |
| Z* | 5V | 3.3V | OK |
| Z* | 0V | 0.1V | OK |
| Z* | Z, 3.3V | 3.2V | OK|

ちなみにおそらく * を付けたパターンはそもそも考えなくて良いのではないかと思っている。TX は
デフォルトで High で待機だと思われるから TOOL 送信中の TX は High と思われる。また出力ピン
である TX に Zモードがあると思えない。

まあ、本体が無事に UART として動いてからでいいだろう。

本体部品表 

| 記号 | 品目、品番等 | 個数 |
| ---  | ---          | ---  |
| B1   | ユニバーサル基板 11x19P | 1 |
| C1,2 | セラコン 0.22uF         | 2 |
| C3   | セラコン 1.5uF          | 1 |
| D1   | ツェナーダイオード 3.3V | 1 |
| J1,2 | ピンソケット 12P        | 2 |
| J3   | USB typeB コネクタ      | 1 |
| R1   | 1.5kΩ                  | 1 |
| R2,3 | 75Ω                    | 2 |
| R4   | 300Ω                   | 1 |
| R5   | 10kΩ                   | 1 |
| S1   | 低背ピンソケット 7P     | 1 |
| S2   | 低背ピンソケット 4P     | 1 |
| S3   | 低背ピンソケット 2P     | 1 |
| S4   | 低背ピンソケット 6P     | 1 |
| U1   | PL2303HXD DIP モジュール | 1 |

PL2303HXD DIP モジュール 

| 品目、品番等             | 個数 |
| ---                      | ---  |
| PL2303HXD IC             | 1    |
| SSOP28 変換基板          | 1    |
| 低背ピンヘッダ 14P       | 2    |

RL78/G24 ボード書き込みアダプタ

| 記号 | 品目、品番等             | 個数 |
| ---  | ---                      | ---  |
| B2   | ユニバーサル基板 11x7P   | 1    |
| D2   | ショットキーダイオード   | 1    |
| R6   | 10kΩ                    | 1    |
| R7   | 300Ω                    | 1    |
| H1-3 | ピンヘッダ 2P            | 3    |
| H4-6 | ピンヘッダ 1P            | 3    |

結果:

``dmesg`` でこんな感じでだめだった。一番不可解なのは 3.3V LDO 出力ピン 17番に 5V が出てい
たこと。

```
usb 1-3-port1: attempt power cycle
usb 1-3.1: new full-speed USB device number 43 using xhci_hcd
usb 1-3.1: Device not responding to setup address.
usb 1-3.1: Device not responding to setup address.
usb 1-3.1: device not accepting address 43, error -71
usb 1-3.1: WARN: invalid context state for evaluate context command.
usb 1-3.1: new full-speed USB device number 44 using xhci_hcd
usb 1-3.1: Device not responding to setup address.
usb 1-3.1: Device not responding to setup address.
usb 1-3.1: device not accepting address 44, error -71
usb 1-3.1: WARN: invalid context state for evaluate context command.
usb 1-3-port1: unable to enumerate USB device
```

あと
[データシート](https://aitendo3.sakura.ne.jp/aitendo_data/product_img/ic/inteface/PL2303/PL2303HXD.pdf)
によると、型番の刻印は 3行あり、

``` 
PL-2303HX
LFxxxxxxD
Txxxxxxxx
```

となっていて、2行目の最後の D が リビジョンD らしいのだが、

```
PL-2303HXD
LF09192
TP47143LD
```

となっている。やられたか？
