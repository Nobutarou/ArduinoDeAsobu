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
