# Arduino でオシロスコープ

ここにあるのをそのままやってみよう、というだけ。

https://www.iizuka.kyutech.ac.jp/faculty/physicalcomputing/pc_kitscope

## シールド化

利用ピンが左側が A0, GND, A1, A2, A3 で問題の右側が D6, D5, D3 のため普通のユニバーサル基
板で位置が合う。

![シールド設計図](./librecad/Ossillo_1.png)

と思ったけど余っている 300Ωを使おう。この 1kΩはおおよそで良いはずだ。一方で 100kΩのは、
10kΩを 10本差す場所はさすがにないので、そのままにする。

![シールド設計図 2](./librecad/Ossillo2.png)

位置決めのために、あえて使わない A5, A4 にもピンヘッダをかます。

入出力はチェック端子を IC クリップやみのむしなどで摘む形にする。
