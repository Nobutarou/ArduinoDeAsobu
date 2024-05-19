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
