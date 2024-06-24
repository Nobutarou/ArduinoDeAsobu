MiniCore

https://github.com/MCUdude/MiniCore

のテスト

# ArduinoISP は動くのか

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
