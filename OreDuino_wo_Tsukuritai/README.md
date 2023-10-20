# 自分で Arduino を作ってみたい

## ブートローダ書き込み済を使う

いきなり素の Atmega328P は荷が思いので 

https://akizukidenshi.com/catalog/g/gI-12774/

で書き込みに挑戦するところから

### USB シリアル変換の PL2303 で書き込もうとしたけどあきらめた

USB シリアル変換の PL2303 を持っていたので、

https://www.instructables.com/USB-to-Serial-TTL-Adapter-to-Program-ATmega328p/

を見ながら書き込みに挑戦してみたけど、

```
avrdude: stk500_recv(): programmer is not responding
avrdude: stk500_getsync() attempt 1 of 10：not in sync resp=0x00
```

みたいなのが出てだめだった。自分で壊してしまったのかは分からない。

ただ Windows10 だと、そもそも COM ポートに通信すらできなかった。情報の多い Windows ですら、
ほぼ情報が見つからないので、この方法は諦める。

### Arduino UNO R4 を書き込み器としてみる。

https://qiita.com/Potewo/items/33380a4b2d787284895a

を見ながらやってみたけど、R4 に Arduino ISP プログラムの書き込み自身はできたけど、動作はし
てくれなかった。
