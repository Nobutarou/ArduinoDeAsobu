# コンデンサを 2 個直列にしたら

```
GND - CT - CB - V0
```

CT: 測定対象の静電容量
CB: 基準静電容量

CT, CB への電圧をそれぞれ VT, VB とする。

$` VT+VB=V0 `$  ...(1)

$` CT \cdot VT = CB \cdot VB `$ ... (2)

(2) は

$` T=\frac{VB}{VT}CB `$ ...(3)

VT を測定する場合、(1)は

$` VB=V0-VT `$ ...(4)

(3)(4) から

$` CT=\frac{V0-VT}{VT}CB `$ ...(5)

プログラム節約するために常数を出しておくなら

$` CT=\frac{V0 \cdot CB}{VT} -CB `$ {6}
