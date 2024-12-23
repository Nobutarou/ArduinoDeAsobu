# 秋月 最大30V出力 昇圧型スイッチング電源モジュール NJW4131使用 の研究

気づいたことをつらつらと

## 仕様書の出力電圧の式の基準値は V_B

[仕様書](https://akizukidenshi.com/goodsaffix/njw4131.pdf) P19 で入力電圧 V_IN に対する倍
率と思い込んで 2kΩの可変抵抗にすれば、13V あたりまで調整できると思い込んだが、正しくは、
基準電圧 V_B に対する倍率で、基準電圧の標準値は 1V.

可変抵抗を 2kΩにしたら、出力電圧計算値は 2~3V にしかならず、入力電圧に届かないから入力電
圧そのものが出ていた。

ちなみに秋月と仕様書の適用回路は R1, R2 が逆になっている。

## ダイオード ZD1 は無くても動く

秋月の回路には保護用にダイオード ZD1 が入っているが仕様書 P17 の適用回路には無いので、前述
の勘違いで動かなかったのを、このダイオードのせいかと思って壊してみたが、普通に動いた。

モーターとか使う人だと急停止したときとかに逆起電力のキツいのが立ったりするのだろうか。

## 多回転ボリュームは感度が低すぎる。

もともと 30V あたりまでを考えてる設計なので、抵抗が大きすぎるのだけど、電圧を 5~13V くらい
で変化させるのに何周もしなくてはならない。半固定抵抗の方がそのあたりは楽そう。基板で足の位
置を揃えてやれば挿せる。

ちなみに残念ながら Bourns  ではなく中華だった。残念。

## 秋月ので 12V あたりを最大にするには

P19 の式から R1, R2 は逆だが仕様書に準拠して

$$\left(\frac{V_{OUT}}{V_B}-1\right) R_1$$

なので VOUT=12V, VB=1V, R1=3.3kΩとすると 36.3kΩとなる。Tocos の半固定抵抗で見ると 30kΩ
か 50kΩとなる。微妙

良く考えたら、ファン用に 5~12V あたりを変化させたいだけなので, 20kΩの半固定抵抗と、20kΩ
の固定抵抗なら 7~13V でちょうど良い感じになりそうだ。もう少し汎用的な 22kΩの固定抵抗でも
8~14V とまずまずだろう。8V 以下に絞りたいなら、始めからそういう低速ファン買う方が良い気も
するし。

## DC12V ファン調整用抵抗ユニット

[設計図](./librecad/register_unit_for_njw4131.pdf)

部品表

| 記号 | 品目                                              | 個数 |
| ---  | ---                                               | ---  |
| PH1  | ピンヘッダ 3P                                     | 1    |
| R1   | 22kΩ                                             | 1    |
| R2   | 可変抵抗 20kΩ, GF063P B203K と同じピン配置のもの | 1    |
| UB1  | ユニバーサル基板 3x6P                             | 1    |

