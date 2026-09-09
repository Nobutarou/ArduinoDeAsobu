MiniCore

https://github.com/MCUdude/MiniCore

のテスト

# ArduinoISP は動くのか (結論: 動く)

手順

- ASOBoard 1台目は OptiBoot (Arduino のブートローダ) 
  - ArduinoISP を書き込む
- ASOBoard 2台目に MiniCore ブートローダを 1台目から書き込む。
  - Arduino ISP を書き込む
- ASOBoard 3台目に 2台目から MiniCore ブートローダを書き込む。
  - ここでエラーとなった。

```
tk500_getsync() warning: attempt 1 of 10: not in sync: resp=0xa0
```

- ASOBoard 3台目に 1台目から MiniCore ブートローダーを書き込む。
  - 成功

このことから MiniCore では ArduinoISP は使えないようだ。

つまり Arduino (OptiBoot) 自体を無くすことは、危険だと言うことだ。

というのは全て間違いだった。

ここによると、RST-GND に 10uF のコンデンサを入れたら動いたとある。

https://github.com/MCUdude/MiniCore/issues/271

さらにここによると要は USB シリアル変換器を遠しているボードは 10uF 入れてリセットを防げと
ある。

https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP/?queryID=undefined

つまり OptiBoot はボーレート 115200 で avrdude コマンドは 19200 を発行しているから握手でき
ずに、ArduinoISP プログラムが起動する。こっちは 19200 で握手できるようになっている。なので
OptiBoot の場合、コンデンサを挟まなくても動作した。

urboot の場合、ボーレートは可変なので avrdude コマンドと握手してしまう。そのため
ArduinoISP が起動せずエラーとなってしまう、ということだと思う。

# ボード選択を Arduino UNO で書き込めるのか

Blink で試す。できない。次のエラーが出る。

```
avrdude: stk500_getsync() attempt 6 of 10: not in sync: resp=0x77
```

# ボーレート変更できる?

urboot ブートローダの説明によると UART はソフトウェアで実装していて、可変とのこと。多分そ
れのおかげで OptiBoot でアップロード失敗しても MiniCore だとできたりする。

https://github.com/stefanrueger/urboot

アップロードテストは Blink とする。ASOBoard は 16MHz 駆動である。

| ボーレート [bps]    | 結果 |
| ----------          | ---- |
| 230400              | OK   |
| 125000              | NG   |
| 115200 (デフォルト) | OK   |
| 57600               | OK   |
| 38400               | OK   |
| 19200               | OK   |
| 9600                | OK   |
| 4800                | NG   | 

125000 は多分 20MHz とかの人向けだろう。大量の商品作成とかしてなければ、正直 9600 や 19200
で十分な気がする。
