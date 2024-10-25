# USBasp の自作

本家

https://www.fischl.de/usbasp/

自作してる人はたくさんいて、その中で参考にしたのがこちら。マイコンのシンボルが実機と同じ配
置になってて、見易いから。

http://www.cs.shinshu-u.ac.jp/~haeiwa/m-com/USBasp/USBasp_new.html

それから、この方。非常に簡略化されていて、必要な部分が分かりやすい。

https://deltav-lab.techblog.jp/archives/30113269.html

最初に思った感想が、なんで USB 機能のあるマイコンを使わないんだろうってところ。

## ver 1.0

まず分からないのが D-, D+ に配置されている定電圧ダイオード。

https://www.falstad.com/circuit/circuitjs.html

で計算しても、電圧は 3.3V か 0V で信号レベル変換にはなっていない。
[回路ファイル](./misc/USB-D_Diode_why.txt)

![D が High](./misc/USB-D_High.png)

![D が Low](./misc/USB-D_Low.png)

寄生インダクタンスによる電圧のオーバーシュートが恐いとしても、信号ラインにダイオード入れて
るのは初めての経験。なんとなくだけど 5V マイコンのために昇圧しようと考えて入れたけど、実は
働いていないということなのではないだろうか。

その先の抵抗も分からない。

そもそも Arduino Uno R3 の Tx, Rx に何故抵抗が入っているのかも良く分からない。他から Tx,
Rx 使われたときの保護とある。まあ一応他が High で自分が Low のとき、理想的な回路なら無限の
電流が流れるから、それに対する保護かな。ただマイコンのピンは対して電流流せないから、そんな
ことしても、High 側の電圧が下がるだけだとは思うけど。

https://forum.arduino.cc/t/resistors-on-the-rx-and-tx-lines-of-the-arduino-pro/154783/4

少なくとも、本機で、2個以上の信号線が混ざることはないから、保護目的なる抵抗は不要だ。

RESET をヘッダに出しているのは、本機をプログラムするときのためのもののようだ。本機はブレボ
で書き込むから、これも不要だ。

そんなこんなで
[回路図1.0](./kicad/USBasp_1.0/USBasp_1.0.pdf)


