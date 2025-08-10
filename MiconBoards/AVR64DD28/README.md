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


