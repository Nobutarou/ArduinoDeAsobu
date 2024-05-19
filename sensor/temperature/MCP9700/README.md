# MCP9700

秋月で MCP-9700-E/TO が売っている。

https://akizukidenshi.com/catalog/g/g109692/

データシートはこちら。

http://ww1.microchip.com/downloads/en/devicedoc/20001942g.pdf

# 仕組み

5V 程度の電力を供給すると VOUT ピンから温度に応じた電圧が出力される。

式4-1 より

$` T_A = (V_{OUT} - V_{0℃} )/T_C `$

ここで

$` T_A `$ : 測定温度

$` V_{OUT} `$ : 出力電圧

$` V_{0℃} `$ : 0℃での出力電圧

$` T_C `$ : 係数 

であり、1節より $` V_{0℃} = 500[mV] `$, $` T_C = 10[mV/℃] `$ である。 

# PIC16F1455 で測定チャレンジ

まず始めに、こんなことおそらく USB 機能のある PIC でやることじゃないと思う。選定理由は他に
手持ちがないというだけである。

測定したいのは 20℃から 25℃あたりで、間違っても 0℃や 50℃を測る気がないとすると、図 2-16
より、0.8V あたりがしっかり計測できて、範囲としては 0.5 ~ 1.0V くらいで良いとなる。

他の PIC もそうだと思うけど内蔵参照電圧 1.024V というものがあるから、これが丁度良いだろう。

RA4 ピンを ADC に使い、RCピンに温度を 2進数で出力してみよう。

[ソース](./pic/pic16f1455/main.c)

[回路図](./kicad/MCP9700/MCP9700.pdf)

Dyson が 25℃と言っている部屋で 26~27℃。まずまずでは。

# 総評

それなりに使い安い。

PIC だと複数の内蔵参照電圧が用意されているから、狙いの温度帯を正確に測りやすい。PIC16F1455
だと 1.024, 2.048, 4.096V が選択できる。

ATMega328P は 1.1V なので計測範囲は限定される。

もちろん精度が適当で良ければ USB からの 5V (4.8V くらいが実態かな) や 3.3V (おそらくまずま
ずの精度はある) を使えば良い。 

式が簡単な四則演算なので、数学の苦手な人でも扱いやすいだろう。