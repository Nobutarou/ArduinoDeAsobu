# リチウムイオン充電器の自作

Bonton で made in Japan と書かれた 18650B を入手したので、充電器を作ってみよう

# v1.0

Torex の IC, 
[XC6802A42XMR-G](https://product.torexsemi.com/ja/series/xc6802)
の SOT-25 パッケージを使う。
[マルツ](https://www.marutsu.co.jp/pc/i/2585649/)
で買える。

Rsen だけど Ibat を 800mA 以下にするので 1.3k ~ 2.0k くらいで入手しやすい抵抗で。

Ibat = 1.0/Rsen * 1000 [A] なので、1.3kΩで 770mA, 2.0kΩで 500mA となる。

[回路図](./kicad/LiIonCharger1.0/LiIonCharger1.0.pdf)

