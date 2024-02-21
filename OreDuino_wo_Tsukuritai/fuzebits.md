# ft232rl シリアル通信で書き込めない

## ヒューズビットの確認

http://hello.world.coocan.jp/ARDUINO9/arduino9_4.html

の最後の方の FT232RL で書き込む方法にに従う。

なぜか hfuse と lock fuse が違う。cali は内部推奨のキャリブレーションに関する値だそうなので、異なってい
て問題ない。

PIC のコンフィグビットと違い、読みにくいので、こちらのウェブアプリを頼る。

https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p

動く方は hifuse は EESAVE=1 となっていて chip erase で EEPROM 消さないのと、lockbit はスケッチとメ
モリにロックはないが、ブートローダはロックしている様だ。

動かない方は hifuse で EESAVE=0 で lockbit は完全フリー。正直これがシリアル通信失敗の原因
とは思えないんだけど。

### シール付でシリアル通信書き込みできるやつ

fuse は efuse, hfuse, lfuse の順番。

```
OK: serial programming mode
OK: ATmega328P detected
RDY
OK+ fuse: 0xfd 0xd6 0xff
OK+ lock: 0xcf
OK+ cali: 0x82
OK: sign: 0x1e 0x95 0x0f 0x00
```

### シール付のシリアル通信書き込みに失敗するやつ

```
OK: serial programming mode
OK: ATmega328P detected
RDY
OK+ fuse: 0xfd 0xde 0xff
OK+ lock: 0xff
OK+ cali: 0x91
OK: sign: 0x1e 0x95 0x0f 0x00
```

### シール無し（失敗する）

```
OK: serial programming mode
OK: ATmega328P detected
RDY
OK+ fuse: 0xfd 0xde 0xff
OK+ lock: 0xff
OK+ cali: 0x82
OK: sign: 0x1e 0x95 0x0f 0x00
```

## avrdude でヒューズビットの書き換えに挑戦

前節のシール無しで調査をする。

https://blueeyes.sakura.ne.jp/2017/07/19/78/

の結線を参考に、というか /etc/avrdude.conf にも書いてあるので見て使う。

```
programmer
    id                     = "ft232r";
    desc                   = "FT232R based generic programmer";
    type                   = "ftdi_syncbb";
    prog_modes             = PM_TPI | PM_ISP;
    connection_type        = usb;
    reset                  = 4;  # DTR
    sck                    = 0;  # TxD
    sdo                    = 2;  # RTS
    sdi                    = 1;  # RxD
```

sdo は ATmega328P では MOSI, sdi は MISO のようだ。統一してくれ。

ちなみに Arduino 使わずに FT232RL でそのままヒューズビットの確認が可能だった。

```
avrdude -c ft232r -p m328p -U hfuse:r:-:h -U lfuse:r:-:h -Ulock:r:-:h
```

先のサイトは ``-t`` で対話モードを使っていたが、本家が非推奨ということで、

https://synapse.kyoto/tips/ArduinoISP_AVRWriter/page002.html

を読みながら、期待通りに動くものと同じ設定にしてみる。

```
echo "0xd6" > hfuse.txt
avrdude -c ft232r -p m328p -B4800 -Uhfuse:w:hfuse.txt
echo "0xcf" > lock.txt
avrdude -c ft232r -p m328p -B4800 -Ulock:w:lock.txt
``` 

期待通りに書き込めた。

```
> avrdude -c ft232r -p m328p -B4800 -Ulfuse:r:-:h -Uhfuse:r:-:h -Uefuse:r:-:h -Ulock:r:-:h
avrdude warning: programmer arduino overwrites previous definition /etc/avrdude.conf:552. [/home/snob/.avrduderc:5]
avrdude: AVR device initialized and ready to accept instructions
avrdude: device signature = 0x1e950f (probably m328p)

avrdude: processing -U lfuse:r:-:h
avrdude: reading lfuse memory ...
avrdude: writing output file <stdout>
0xff

avrdude: processing -U hfuse:r:-:h
avrdude: reading hfuse memory ...
avrdude: writing output file <stdout>
0xd6

avrdude: processing -U efuse:r:-:h
avrdude: reading efuse memory ...
avrdude: writing output file <stdout>
0xfd

avrdude: processing -U lock:r:-:h
avrdude: reading lock memory ...
avrdude: writing output file <stdout>
0xcf

avrdude done.  Thank you.
```

先の節の Arduino スケッチでも、狙い通りになった。

```
OK: serial programming mode
OK: ATmega328P detected
RDY
OK+ dmpb
OK+ fuse: 0xfd 0xd6 0xff
OK+ lock: 0xcf
OK+ cali: 0x82
OK: sign: 0x1e 0x95 0x0f 0x00
```

## シリアル通信 (Arduino IDE からの) で書き込めるかテスト

何度も読んだこれだけど

https://ht-deko.com/arduino/atmega328p.html

やはり書き込めなかった。リセット出きてない気がするんだよね。

でも avrdude で ft232r プログラマで普通に書き込めた。
しかし動作がおかしい。

```
# 光らない
avrdude -v -V -p m328p -c ft232r -D -Uflash:w:Blink.ino.hex:i

# 3回すばやく点滅を繰替えす。まるで Arduino 起動時の動作を繰り返しているみたい
avrdude -v -V -p m328p -c ft232r -D -Uflash:w:Blink.ino.with_bootloader.hex:i
```

``-D`` オプション、フラッシュメモリを消去しない、を消したところ、期待通りの点滅をした。

```
avrdude -v -V -p m328p -c ft232r -Uflash:w:Blink.ino.with_bootloader.hex:i
avrdude -v -V -p m328p -c ft232r -Uflash:w:Blink.ino.hex:i
```

そして一度上手く行った後は ``-D`` 付けても動いた。いろいろやってる間の出来事なので、単に配
線ミスだったのかもしれない。ちょっと不明。

これはヒューズビットを書き替えたシール無しにも、そのままにしたシール有りにも有効だったので、
ヒューズビットは完全に無罪となった。

結局、シリアル通信のはリセットに失敗してるんじゃないかなあ。手持ちの ATmega328P 4個で、
2個がシリアルプログラミングに成功して、2個が失敗している。

んー FT232RL を使い回すことで安価な Arduino を作るのが目的だったから、なんか簡単に SPI と
シリアルを切り替えられる仕掛けが必要になりそうだ。
