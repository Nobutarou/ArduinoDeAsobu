# PIC で USB シリアルアダプタを作る

こちらの方が PIC16F1455 で作っている。そしてこの機種は a-p-prog 対象機種である。

https://elchika.com/article/a4b16182-6270-493c-b4da-7c519ac3429c

## MPLAB IDE でビルドできるのかの確認

MCC は Classic を使うと同じ画面が出た。また USB_device_CDC.c の 117行目の書き換えが必要と
書いてあるが、似ている部分がない。ただし 117行目というのがちょうど関数定義の始まる部分であ
ることと、その 3行に出ている文字列で ```grap -r``` しても ```#define``` されている気配が
ないことと、```if #defined``` で使われていることから、自力で ```#define``` すれば良いのか
と言う気がしたので 117行目あたりに自力で書いてみた。

```C
#if defined(USB_CDC_SET_LINE_CODING_HANDLER)
CTRL_TRF_RETURN USB_CDC_SET_LINE_CODING_HANDLER(CTRL_TRF_PARAMS);
#endif

/** ここから
#define USB_CDC_SUPPORT_DTR_SIGNALING
#define UART_DTR PORTCbits.RC3
#define USB_CDC_DTR_ACTIVE_LEVEL 0
/** ここまで

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void USBCDCSetLineCoding(void);
```

ビルドは通った。なのであとはブレッドボードで試してみるしかない。

現時点で必要な部品

- USB コネクタ。
- PIC16F1455
- セラコン 0.1uF

また、こんなプロジェクトもあった。フルのソースも hex も付いているので、まあ、最初に参照し
たのがだめでもなんとかなるだろう。

https://github.com/Calcousin55/PIC16F145x_USB2Serial
