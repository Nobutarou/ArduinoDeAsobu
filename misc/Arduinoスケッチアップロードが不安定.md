https://support.arduino.cc/hc/en-us/articles/4401874331410--Error-avrdude-when-uploading

基本、ブートローダをもう一度書き込めとか書いてあるけどそれだけじゃない。

自分の場合、

- MiniCore なら書き込める確率が高い
- USB 全部抜いたら書き込める

などがおきていて、これはブートローダーが不適切云々では説明ができない。

そこで、いろいろテストしてみた。繰替えしテストしているので同じパターンもある。どうやら一番
怪しいのは GL850G の 3代目の USB ハブが何か怪しいということが分かった。他の USB 機器は普通
に動いているのに、Arduino (ではないけど) だけ変なようだ。

逆に言えば、Arduino のスケッチアップロードで USB ハブの安定性を測れそうだ。

他にも boards.txt で baudrate の指定はできるが、多分 OptiBoot 側で機種毎に決め打ちしてそう
で、下げても動かない。

| Hub1           | Hub2       | Switcher | Self power | USB-UART   | USB Speed [bps] | Cable Shield | Bootloader | Baudrate | Result | Comment                                              |
| -------------- | ---------- | -------- | ---------- | ---------- | --------------- | ------------ | ---------- | -------- | ------ | ---------------------------------------------------- |
| Elecom U3SW-T4 |            | on board | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| Elecom U3SW-T4 | GL850G-3rd | on board | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-3rd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     | 挿入時にカーネルエラーが出るときがある。その場合は不明                          |
| GL850G-3rd     |            | no       | yes        | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | NG     | avrdude: stk500_recv(): programmer is not responding |
| GL850G-3rd     |            | no       | yes        | PIC16F1455 | 12M             | Yes          | OptiBoot   | 9600     | NG     | avrdude: stk500_recv(): programmer is not responding |
| GL850G-3rd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-3rd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 57600    | NG     | おそらく OptiBoot が決め打ちしている                              |
| GL850G-3rd     |            | yes      | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | NG     |                                                      |
| GL850G-3rd     |            | yes      | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-3rd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-3rd     |            | no       | no         | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | OK     | 挿入時にカーネルエラーが出るときがある。その場合は不明                          |
| GL850G-3rd     |            | no       | yes        | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | NG     |                                                      |
| GL850G-3rd     |            | no       | yes        | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-3rd     |            | no       | yes        | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | NG     |                                                      |
| GL850G-3rd     |            | no       | no         | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | NG     |                                                      |
| GL850G-2nd     |            | no       | no         | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | yes      | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | yes      | no         | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | OK     | 挿入時にカーネルエラーが出るときがある。その場合は不明                          |
| GL850G-2nd     |            | no       | no         | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | no       | no         | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | no       | yes        | PIC16F1455 | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
| GL850G-2nd     |            | no       | yes        | FT2332RL   | 12M             | Yes          | OptiBoot   | 115200   | OK     |                                                      |
