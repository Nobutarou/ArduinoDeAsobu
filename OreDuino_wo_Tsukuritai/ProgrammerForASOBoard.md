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

FT232RL 側のアダプタの回路図はこちら。

[FT232RL アダプタ回路図](./ProgrammerForAsoBoard/kicad/FT232RL_Adaptor/FT232RL_Adaptor.pdf)

設計図はこちら。なるべく現行の延長線上で。

![FT232RL アダプタ設計図 1.0](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_v1.0.png)

配線済み基板切削図はこちら。左右反転済みなので、そのまま彫れば良い。

![FT232RL アダプタv1.0 切削図](./ProgrammerForAsoBoard/librecad/FT232RL_Adaptor_v1.0_board_reversed.png)

部品表

| 記号 | 品目           | 個数 |
| ---  | ---            | ---  |
| D1   | LED            | 1    |
| J1,2 | 3ピン XH ヘッダ| 2    |
| J3   | 2ピン XH ヘッダ| 1    |
| JR1,2| ジャンパー     | 2    |
| S1,2 | 12ピンソケット | 2    |
| -    | ユニバーサル基板 15x10 | 1 |
