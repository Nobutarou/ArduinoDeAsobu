# 余っている PIC を I/O エキスパンダ MCP23017 としてみたい

MCP23017 見てて、余ってるマイコンでできそうだよなあと思っていたら、
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

# もう一度 MCP23017 擬態に挑戦

PPS が必要ということが分かったのでデフォルト？ピン配置にこだわる必要がない。RA3 はどのみち
使えないので、RA4, RA5 を I2C に使い、RC ポートを全部 IO にするほうが素直な気がする。つま
り GPIOA に RA0~RA2, GPIOB に RC0~RC5 を配置する。

また ```-std=c90``` が必要だったところを直しておきたい。

[ソース](./pic16f18323/true_main.c)

Adafruit MCP23017 ライブラリを使ったテストで使用可能な全 9ピンの digitalWrite を確認。
[ソース](./arduino/mcp23017_blink/mcp23017_blink.ino)

https://youtu.be/OH9FeNeptBQ

LCD LM162AT1 も動かせた。ライブラリはこちら

https://github.com/ATrappmann/LiquidCrystal_MCP23017_I2C/tree/master

[ソース](./arduino/lcd_hello/)

![テスト](./arduino/lcd_hello/pic16f18323_as_mcp23017_lcd.jpg)
