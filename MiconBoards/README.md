# はじめに

いろんなマイコンで開発ボードを作ってみたい。

# R7FA4M1AB3CFM

秋月で DIP 化されたものが売っている。Arduino UNO R4 のマイコン。二列の配列からどうやって、
取り出すかが課題。

https://akizukidenshi.com/catalog/g/g129397/

[ここ](./R7FA4M1AB3CFM.md) でやってみたが、今のところ上手くできていない。

# AVR64DD28

秋月で ATMega348P よりも安い。

https://akizukidenshi.com/catalog/g/g118314/

[DxCore](https://github.com/SpenceKonde/DxCore) がサポートしている。

# PIC1827Q43

秋月で、ATMega328P よりも高性能で安い AVR64DD28 よりも安いのに、さらに高性能？

https://akizukidenshi.com/catalog/g/g116593/

| 特性 | ATMega328P | AVR64DD28 | PIC18F27Q43 |
| --- | --- | --- | --- |
| 秋月税込価格 2025.09.03 | 400 | 350 | 330 |
| プログラムメモリ | 32kB | 64kB | 128kB |
| ADC 解像度 | 10bit | 12bit | 12bit |
| クロック | 外付け 20MHz | 内蔵 24MHz | 内蔵 64MHz | 

ATMega や AVR の Arduino みたいに気楽に使えるようにできるなら、気楽というのは自分的には、
ちょっとプログラムを書いたら Serial.print() で確認しながら、ちまちま進めるというやりかたが
できるのであれば、面白いと思う。
