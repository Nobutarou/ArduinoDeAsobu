# 情報

本家: https://littlewire.github.io/
回路: https://github.com/littlewire/Little-Wire/tree/master/hardware
作ってみた人の解説:
https://yueno.net/xoops/modules/xpwiki/?%E9%96%8B%E7%99%BA%2FAVR%2FAVR%20Writer%2FUSBtinyISP

ATTiny85 の RST ピンを普通のピンにするためのヒューズ設定をするので、一発勝負か。ま何事も経
験。

# 1.0, とりあえず

自作 USBasp と同じピン配置にする。

https://github.com/Nobutarou/ArduinoDeAsobu/tree/main/USBasp#ver-11

USBasp は ASOBoard での Arduino as ISP と同じ配置なので、アタッチメントが共通。

ATTiny85 の PB5 は初回のブートローダ書き込みまでは RST で、Little Wire ファームウェアを書
き込むと SS となる。ジャンパで切替えるようにする。

電源による LED 点灯は省く。データ送信に合わせてチカチカするなら採用するけど、5V 入ってるか
どうかは簡単に確認できるので。

本家では 3.3V 駆動のマイコンのためにジャンパで電源入力を変更できるようになってるが、本機は
アタッチメント方式なので、アタッチメント側でやれば良い。まあ多分使うことはない。

D-, D+ の抵抗は USBasp のときは R1=R2=75Ω、R3=2kΩだった。Falstad での練習でも 27Ωだと
USB 側が Low になったときに電流が流れすぎる気がする。そういうことがあるのかどうかも知らな
いけど。このへんは、手持ちと買い物の事情でかなり適当にやっても大丈夫な気がする。

https://tinyurl.com/ywh4brd8

[回路図](./kicad/LittleWire1.0/LittleWire1.0.pdf)

で、[設計図](./libracad/LittleWire1.0.pdf)

部品表

| 記号 | 品目、型番              | 個数 | 備考 |
| ---- | ----------              | ---- | ---  |
| B1   | ユニバーサル基板 14x19P | 1    |      |
| C1,2 | 0.1uF 程度のセラコン    | 2    | 2個も必要か？ |
| D1,2 | ツェナーダイオード 3.6V | 2    | 3.3V とか 3.9V でも良いか？ |
| H1,2 | ピンヘッダ 2P           | 2    |      |
| J1   | USB B コネクタ          | 1    |      |
| R1,2 | 27Ω                    | 2    | 75Ωとか 300Ωとかでも良いか？|
| R3   | 1.2kΩ                  | 1    | 1k ~ 10kΩとかでも良いか？|
| R5   | 10kΩ                   | 1    | 1k ~ 100kΩならなんでも良いか?|
| S1-3 | ピンソケット 2P         | 3    |      |
| S4   | ピンソケット 8P         | 1    | |
| S5   | IC ソケット 8P          | 1    | |
| U1   | ATTiny85-20PU           | 1    | |

## firmware

最初間違えて、いきなり LittleWire を書き込んでしまった。ここからダウンロードして

https://littlewire.github.io/downloads.html


```sh
avrdude -c usbasp -p t85 -U flash:w:littlewire_v13.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
```

としてしまった。更新する気がないなら、多分大丈夫。

本来は 

```
git clone https://github.com/micronucleus/micronucleus.git
cd micronucleus\firmware\releases
avrdude -c usbasp -p t85 -U flash:w:t85_default.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
```

ただ、どちらも USB として認識しない。検索するとクリスタルの周波数が違うとそうなる、と出てきた。

```
[13427.503847] usb 1-3.2: new low-speed USB device number 70 using xhci_hcd
[13427.531685] usb 1-3.2: Device not responding to setup address.
[13427.765692] usb 1-3.2: Device not responding to setup address.
[13427.973849] usb 1-3.2: device not accepting address 70, error -71
[13427.981498] usb 1-3-port2: unable to enumerate USB device
```

駄目元で二個目のパスコンを追加したがやっぱり駄目。水晶には関係ないからね。

何度も抜き差しすると、一度だけ認識。

```
[14548.454706] usb 1-3-port2: attempt power cycle
[14549.043875] usb 1-3.2: new low-speed USB device number 89 using xhci_hcd
[14549.143362] usb 1-3.2: New USB device found, idVendor=16d0, idProduct=0753, bcdDevice=
2.06
[14549.143365] usb 1-3.2: New USB device strings: Mfr=0, Product=0, SerialNumber=0
```

なんとポートを変えると動いた。LittleWire を直接書き込んでしまった方も動いた。ポートを変え
ずに、そのポートに HUB を挿して、そこに挿しても動いた。

```
[15157.717225] usb 1-3.2.4: new low-speed USB device number 29 using xhci_hcd
[15157.946937] usb 1-3.2.4: New USB device found, idVendor=1781, idProduct=0c9f,
bcdDevice= 1.04
[15157.946940] usb 1-3.2.4: New USB device strings: Mfr=0, Product=2, SerialNumber=3
[15157.946941] usb 1-3.2.4: Product: USBtinySPI
[15157.946942] usb 1-3.2.4: SerialNumber: 512
```

一つ判明したのは自作 PS/2-USB アダプタと同じハブからだと動かない。

https://github.com/Nobutarou/ArduinoDeAsobu/tree/main/PS_2-to-USB_KB

なぜか

- PS/2-USB アダプタの電源 off
- LittleWire 挿す
- PS/2-USB アダプタの電源 on

だと問題が出ない。Arduino IDE からも書き込めた。

udev に rule が必要。[99-USBtinyISP.rules](./99-USBtinyISP.rules)


