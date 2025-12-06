# v1.0 NJL7502 + RL78/G24

M5Stack用環境光センサユニット M5STACK-U136 が良いかなあと思ってたけど、NJL7502L で似たよう
なことができそう。より手作り感が出るので、こっちでやってみる。

こちらがデータシート
https://www.nisshinbo-microdevices.co.jp/ja/pdf/datasheet/NJL7502L_J.pdf

P5 の NJL7502L Light Source A Vce=5.0V の 10kΩの Vout-Ev(Lx) 線図を使ってルクスを測定、表
示できそう。家の中は、シーリングも純粋な白ではないだろうし、いろいろ反射していろんな波長が
混ってると思うので、こっちのグラフ。家の中は数100Lx 程度らしいので、マイコンの ADC のレン
ジを考えると 10kΩのラインとなる。

対数同士では、ほぼ線形なので Vout[V] と Ev[Lx] の関係式は、

```math
log(V_{out})=a \, log(E_v) + b
```

Ev=500Lx で Vout=3V, Ev=100Lx で Vout=0.6V から

```math
log(3)=a \, log(500) + b
log(0.6)=10a + b
```

よって

```math
a=\frac{log(3)-log(0.6)}{log(500)-log(100)}=1.0
b=log(0.6)-a\,log(100)=-2.2218487
```

よって Ev[Lx] は次のように書ける。

```math
E_v=10^{\frac{log(V_{out})-b}{a}}=10^{log(V_{out})+2.2218487}
```

検算: OK
- Vout=3V のとき、式のEv=500Lx でプロットと一致
- Vout=1V のとき、式の Ev=167Lx で、多分そんな感じ
- Vout=0.6V で、式の Ev=100Lx で OK
- Vout=0.1V で、式の Ev=17Lx で、だいたい OK

[回路図](./v1.0/LuxMeter1.0/LuxMeter1.0.pdf)
