# PS/2 キーボードを USB に変換するアダプタ

## PIC16F1459 版

ソースはこちら、https://github.com/BojanSof/at2usb

そんなにピンを使わないので PIC16F1455 でできないかと思ったが、UART TX の RB7 ピンの内部プ
ルアップを使っていて、PIC16F1455 の UART TX である RC4 は内部プルアップが使えないから、単
に入れかえることができない。(pin_manager.h より)。

のだけど、そもそも UART を何に使っているのかが不明である。pin_manager.h に RB7_某関数がい
ろいろ定義されているが、他で全く使われていない。ということはどうでも良いはずである。

ということで、そのまま MPLAB X IDE でマイコンを PIC16F1455 に変更してみると、

```
mcc_generated_files/pin_manager.c:64:5: error: use of undeclared identifier 'LATB'
    LATB = 0x00;
    ^
mcc_generated_files/pin_manager.c:71:5: error: use of undeclared identifier 'TRISB'
    TRISB = 0x70;
    ^
mcc_generated_files/pin_manager.c:78:5: error: use of undeclared identifier 'ANSELB'
    ANSELB = 0x00;
    ^
mcc_generated_files/pin_manager.c:84:5: error: use of undeclared identifier 'WPUB'; did you mean 'WPUA'?
    WPUB = 0x00; 
```

で定義されている関数は PIN_MANAGER_Initialize() というもので mcc.c の SYSTEM_Initialize()
で使われていて、それは main.c の main() で最初に使われる。しかしビット操作なんだから 16進
じゃなくて 2進で書いてくれる方が読みやすいと思うんだけどなあ。

というとで RB ポートは自分は使わないので RB ポートに関する先のビット操作を全て削除してみる。

あ、通った。

```
mcc_generated_files/tmr2.c:93:: warning: (520) function "_TMR2_StopTimer" is never called
mcc_generated_files/tmr2.c:99:: warning: (520) function "_TMR2_ReadTimer" is never called
mcc_generated_files/tmr2.c:108:: warning: (520) function "_TMR2_WriteTimer" is never called
mcc_generated_files/tmr2.c:114:: warning: (520) function "_TMR2_LoadPeriodRegister" is never called
mcc_generated_files/eusart.c:137:: warning: (520) function "_EUSART_is_rx_ready" is never called
mcc_generated_files/eusart.c:142:: warning: (520) function "_EUSART_is_tx_done" is never called
mcc_generated_files/eusart.c:147:: warning: (520) function "_EUSART_get_last_status" is never called
mcc_generated_files/eusart.c:151:: warning: (520) function "_EUSART_Read" is never called
usb/usb_device.c:1077:: warning: (520) function "_USBStallEndpoint" is never called
usb/usb_device.c:1133:: warning: (520) function "_USBCancelIO" is never called
usb/usb_device.c:1251:: warning: (520) function "_USBDeviceDetach" is never called
usb/usb_device.c:3077:: warning: (520) function "_USBGet1msTickCount" is never called
```

使われてないファンクションが沢山あるみたい。気持悪いのでコメントアウトしていく。

### キー信号変換

信号変換は ps2usb.c でやっていると思う。

[PS/2 スキャンコード](https://www.ne.jp/asahi/shared/o-family/ElecRoom/AVRMCOM/PS2_RS232C/KeyCordList.pdf)

の 0x83 までの HID のキーコードを ps2UsbMap[] に 0x00 から順番に書き込み、それ以降を
ps2ExtUsbMap[] に書き込んでいるように見える。

[HID のコード](https://bsakatu.net/doc/usb-hid-to-scancode/)
の hex 値。JIS 配列のキーも記載がある。

ps2ExtUsbMap は PS/2 スキャンコードで E0 で始まるものだと思う。E0,11 番が E6 右Alt と言う
ことだと思う

ps2_keyboard.h にいろいろ定義されてるけど、全然使われてないので、これは無視して良いと思う。

