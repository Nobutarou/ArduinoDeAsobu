# DTR ラインの C5 の容量テスト

ASOBoard にも入れているが DTR ラインの 0.1uF は、実際はどこまでできるのかテスト。

https://www.arduino.cc/en/uploads/Main/Arduino_Uno_Rev3-schematic.pdf

ASOBoard 2.3

https://x.gd/KpIXD

を使用。並列に継ぐ形式でテスト。

| Added C | Org C | To      | Upload |
| ------- | ----- | ------- | ------ |
| 0.1uF   | 0.1uF | 0.2uF   | OK     |
| 1uF     | 0.1uF | 1.1uF   | OK     |
| 10uF    | 0.1uF | 10.1uF  | OK     |
| 33uF    | 0.1uF | 33.1uF  | NG     |
| 220uF   | 0.1uF | 220.1uF | NG     |

容量を小くすることはできないので、最小の探索はそのうちに。
