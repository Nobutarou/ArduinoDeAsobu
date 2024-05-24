# 概要

MCP9700 の使い方は
[こちら](../../sensor/temperature/MCP9700/README.md)
。

そこでは PIC16F1455 を使っているが USB 機能がある必要がないので、

- ピン本数 14本
  - VDD, VSS, MCLR, センサー入力, LED x5 の 9本で良いが、10本の PIC は無いので
  - MCLR に IO ピンを割り当てれば 8本で良いが、プログラミングに支障とか出てほしくないので
    触らないことにしている
- ADC が 1.024V の内蔵参照電圧を使える

という要件で PIC16F18323 の選定となった。データシートはこちら

https://x.gd/AGjOI

LED 5個で温度を表示する。0~31℃までとなる。室温が 30℃を超えてはならないので 31℃までの表
示で問題ない。

# V1.0

回路図は概要のところの回路図のまま。LED 抵抗は余りまくって使い道のない 75Ωとする。
ASOBoard のように作業するわけではないので、多少眩しくても問題ない。