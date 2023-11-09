# 安価なArduinoの作りかた

信仰上の理由により、中華互換品を除く。

## 秋月の ATmega168／328マイコンボードキット

![ATmega168/328 マイコンボードキット](https://akizukidenshi.com/catalog/g/gK-04590/)

2023年11月時点で 2,100 円と超安価なわけではなく、Arduino そのものではないが、Arduino BIOS
書き込み済の ATmega328P に挿げ替えれば、Arduino Uno R3 互換となる。

![Arduino　Bootloader書込済（5V　16MHz仕様）　ATmega328P](https://akizukidenshi.com/catalog/g/gI-12774/)

なお自分は失敗したが、本機に付属している超有能な USB シリアル変換アダプタ FT232RL は、その
ままだとはんだ付けが必要になる。がんばって結線すれば他でも使えなくはないけど IC ソケットを
買っておいて、取り外せるようにするべきだと思う。

![丸ピンＩＣソケット　（２４Ｐ）　６００ｍｉｌ](https://akizukidenshi.com/catalog/g/gP-00032/)

ボードで開発して、実装は別途ユニバーサル基板などで行なうのであれば、2個目からは Boot 書き込み済み
ATmega328p だけ購入すれば良い。

または、Arduino Uno R3 があれば Arduino ブートローダも書きこめる。

https://ht-deko.com/arduino/atmega328p.html

こちらのサイトが分かりやすい
