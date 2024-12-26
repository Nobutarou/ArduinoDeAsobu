# 余っている PIC を I/O エキスパンダ MCP23017 としてみたい

このようにそのままのことをやってる方がいた。
[こちら](
https://kikyoya.wordpress.com/2017/12/04/
)

PIC16F18323 で見様見真似でやってみたのが、[これ](./pic16f18323/main.c) なんだけど Adafruit
の MCP23017 ライブラリの blink で I2C 通信ができない Error が出る。

https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library

ということで、まずは ASOBoard と PIC16F18323 で I2C 通信を練習するところから始める。

# PIC16F18323 と I2C 接続の練習をする

こちらの方を参考にする。

http://zattouka.net/GarageHouse/micon/I2C/I2C_1.htm

i2c 子機として動いていることを確認するのには

https://docs.sunfounder.com/projects/elite-explorer-kit/ja/latest/appendix/i2c_sacnner.html

こちらを使った。

一つ分かったことは、データシートを読んだ自分の解釈では RC0, RC1 に SCL, SDA が初期割り当て
されていると思っていたが、実際は PPS 設定をしないと動かなかった。

その後、参考にした方のプログラムも動作した。[PIC16F18323 用のソース](
./pic16f18323/i2c2asoboard/)
