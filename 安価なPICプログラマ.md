# 安価な PIC プログラマ

## 経緯

始めは秋月のマイコンボードキットを買うついでに付いてくる FT232RL を使う
![fpicprog](https://github.com/gphalkes/fpicprog) を使うつもりだったが、はんだ付けするよう
に設計されており、別途送料 600円を払ってピンソケットだけ購入するのもなんだったので、そのま
まはんだ付けしてしまった。

![秋月のマイコンボード AE-ATmega](https://akizukidenshi.com/catalog/g/gK-04590/)

うまく結線できれば使えそうだったが、うまく結線するには IC クリップが必要だと感じ、それをま
た購入して待つのもなんだったので Arduino を使う 
![a-p-prog](https://github.com/jaromir-sukuba/a-p-prog) に切り替えた。

## a-p-prog 使いかた

### ハード
Hardware option 1 のところでは結線表 (GND, 5V, A3, A1, A0) ではこの 5線だが、写真では LED,
抵抗 2個が付いている。それらを試しに全部省いても動作した (PIC12F1840 にて)。

5V と MCLR の抵抗 (写真一番手前) だけど、おそらくもともと弱プルアップが入ってる PIC には不要だと思う。

VDD と GND の間のコンデンサは 0.1uF くらい。まあこれはブレッドボードにいつも差しっぱなしな
ので、取り除かなかったけど、普通なくても動くと思う。

奥の抵抗と LED は VDD にちゃんと電圧が来てるか確認するだけだから、無くてもよい。

ということで、写真に惑わされたけど、本当に表にある通りの結線で OK. 一応、結線図も書いてお
いた。交差せず書ける人は本当にすごいですね。

![最小の結線図](./pdf/a-p-prog.pdf)


で、fw/pp フォルダの pp.ino を Arduino に書き込めば、PIC プログラマの完成である。

### ソフト

まず書き込まず、テストをしてみる。

```
./pp3 -c /dev/ttyUSB0 -n -p -t 12f1840 Lchika.X.production.hex
```

で、自分の場合、

```
PP programmer, version 0.99
Serial port failed to receive a byte, read returned 0
/dev/ttyUSB0: Success
zsh: IOT instruction  ./pp3 -c /dev/ttyUSB0 -n -p -t 12f1840 Lchika.X.production.hex
```

となり、失敗することが分かる。何がサクセスなのか知らんけど、間違いなく失敗している。

なので失敗しなくなるまでウェイトを掛ける。自分の場合は 2000ms 待つことにすると、

```
./pp3 -c /dev/ttyUSB0 -s 2000 -n -p -t 12f1840 Lchika.X.production.hex
```

```
PP programmer, version 0.99
Sleeping for 2000 ms while arduino bootloader expires
Wrong device ID: 0000, expected: 1b80
Check for connection to target MCU, exiting now
```

と failed が消えるので、これなら書き込める。

最後に実際に書き込む。


```
./pp3 -c /dev/ttyUSB0 -s 2000 -t 12f1840 Lchika.X.production.hex
```
