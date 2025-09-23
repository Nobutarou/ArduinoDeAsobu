; I2C 接続の LCD_IO 10 は I2C で 7ビットを送ると対応するピンがそのまま High, Low になっ
; て動く。多分 IOエキスパンダの GPIOA だけで完結してるんだと思う。
; こっちは、無理矢理やってるから GPIOA, GPIOB 両方使うので、ピン配置を変えるだけでは対応できない。
; 普通の 4線通信の LCD_IO 4 を変更する

; print, locate, cls, hex に対応
; print の引数は string, long のみ。他は WordToString とかそういうの使え
; hex は単純に文字コード表そのまま描画。
; 他は無視

; GCBasic は全てグローバル変数
; そして <inst_dir>/include 以下はすべて自動読み込み
; つまり自分の知らんうちに変数が使われている
; Ore_ とか付けておくのが無難
; 「？？？」となったときに変数名変えるだけで解決したりする

; 初期設定
; RS = B0 (PIC ではなくて GPIO で記述)
; RW = B5
; EN = B4
; D4 = B2
; D5 = B3
; D6 = B1
; D7 = A2

#DEFINE OreDebug 0; 1: debug out to serial
#define ORE_LCD_WIDTH 16

dim OreLCD_RS_STATE as bit

macro OreLCD_RS(bit)
  OUT_GPB_BIT(DeviceAddress,0,bit)
  #ifdef OreDebug 1
    HSerPrint "RS "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
  OreLCD_RS_STATE=bit
end macro

macro OreLCD_RW(bit)
  OUT_GPB_BIT(DeviceAddress,5,bit)
  #ifdef OreDebug 1
    HSerPrint "RW "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro

macro OreLCD_EN(bit)
  OUT_GPB_BIT(DeviceAddress,4,bit)
  #ifdef OreDebug 1
    HSerPrint "EN "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro

; これらの時間は 270kHz 動作時の話。100kHz ではおよそ 3倍となる。
; イネーブルセットアップ時間 tAS は最小で 140ns。
; イネーブルパルス幅は最小で 450ns。
; イネーブルサイクル時間は最小で 1000ns。
; だがインストラクション実行時間が基本 37us。
; 37us を越えるクリアとカーソルホームは別途 wait を掛けよう

macro OreLCD_EN_PULSE
  wait 1 us ; 1 us >> 140 ns *3
  OreLCD_EN 1
  wait 2 us ; 2 us >> 450 ns *3
  OreLCD_EN 0
  wait 220 us ; 220us >> 37 us * 3
end macro

macro OreLCD_DB4(bit)
  OUT_GPB_BIT(DeviceAddress,2,bit)
  #ifdef OreDebug 1
    HSerPrint "DB4 "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro

macro OreLCD_DB5(bit)
  OUT_GPB_BIT(DeviceAddress,3,bit)
  #ifdef OreDebug 1
    HSerPrint "DB5 "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro

macro OreLCD_DB6(bit)
  OUT_GPB_BIT(DeviceAddress,1,bit)
  #ifdef OreDebug 1
    HSerPrint "DB6 "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro

macro OreLCD_DB7(bit)
  OUT_GPA_BIT(DeviceAddress,2,bit)
  #ifdef OreDebug 1
    HSerPrint "DB7 "
    HSerPrint bit
    HSerPrintCRLF   
  #endif
end macro


Sub ORE_LOCATE (In OreLCDLINE, In OreLCDCOlumn)
'Sub to LOCATE the cursor
'Where OreLCDCOlumn is 0 to screen width-1, OreLCDLine is 0 to screen height-1

    If OreLCDLINE > 1 Then
        OreLCDLINE = OreLCDLINE - 2
        OreLCDCOlumn = OreLCDCOlumn + ORE_LCD_WIDTH
    End If

    ; 0x80 = 0b1000000, or だから先頭の 1 は必ず残る。これは DD RAM アドレスセットのインス
    ; トラクションの合図
    ; 0x40 = 0b0100000。先頭行のアドレスが 0、二行目が 0x40 から始まる。OreLCDLINE=1 となれば
    ; 二行目となる
    ORE_LCDWriteByte(0x80 or 0x40 * OreLCDLINE + OreLCDCOlumn)
End Sub

Sub ORE_CLS
'Sub to clear the LCD
    OreLCD_RS(0)

    'Clear screen
    ORE_LCDWriteByte (0b00000001)
    Wait 4700 us ;1.52ms x3

    ' LCD の左上に文字を出す準備を始めたみたいな感じ
    ; 0x80 = 0b10000000
    ORE_LCDWriteByte(0x80)
    Wait 220 us

End Sub

sub InitLCD
; データシートと読み比べていく

    asm showdebug  `OreLCD`

        ; VDD が 4.5V に到達してから 15ms 以上待つ
        ; 270kHz の話と仮定して 100kHz であれば 
        Wait 47 ms

        ; ORE_RS| R/W | DB7-DB4 | で
        ; 0 | 0   | 0011    | 

        OreLCD_RS 0
        OreLCD_RW 0
        OreLCD_EN 0

        OreLCD_DB7 0
        OreLCD_DB6 0
        OreLCD_DB5 1
        OreLCD_DB4 1

        OreLCD_EN_PULSE

        ; 4.1ms 以上待つ (x3)
        Wait 22 ms
        
        ; 2回目
        OreLCD_EN_PULSE
        
        ; 100us 以上待つ (x3)
        wait 470 us

        ; 3回目
        OreLCD_EN_PULSE

        ; RS| R/W | DB7-DB4 | 
        ; 0 | 0   | 0010    | 4 bit mode の指定

        OreLCD_DB7 0
        OreLCD_DB6 0
        OreLCD_DB5 1
        OreLCD_DB4 0
        OreLCD_EN_PULSE

       '===== now in 4 bit mode =====

        ORE_LCDWriteByte 0x28    '(b'00101000')  '0x28 set 2 line mode
        ORE_LCDWriteByte 0b00001000 ; 表示オフ. 
        ORE_CLS

        ; 0x06=0b00000110, I/D=1 でカーソルは文字の右に, S=0 でカーソルが動く (文字は動かない)
        ; Arduino でも同じ設定なので大丈夫
        ORE_LCDWriteByte 0x06    

        ; datasheet 的には完了
       
        ; 表示を on にしてクリア。まあ残っていても良いか
        ORE_LCDWriteByte 0x0C    '(b'00001100')  'Turn off cursor
        ORE_CLS  

    ; 12 = 0b1100 だけど CLS しちゃってるから 1 だと思うんだけど。
    ; 多分デバグ出力目的で、実動作には使わない？
    ;LCD_State = 12

end sub


SUB ORE_PRINT (In ORE_PRINTData As String)
'Sub to PRINT a string variable on the LCD
  #ifdef OreDebug 1
    HSerPrint "PRINTData "
    HSerPrint ORE_PRINTData
    HSerPrintCRLF   
  #endif

  ; help に従うなら LEN 関数かな
  ; この書式で良いみたいだけど Help に記載がないように思う
  ; https://sourceforge.net/p/gcbasic/discussion/579126/thread/9b18e3fc/
    Ore_PRINTLen = ORE_PRINTData(0)
  #ifdef OreDebug 1
    HSerPrint "PRINTLen "
    HSerPrint ORE_PRINTLen
    HSerPrintCRLF   
  #endif

    If ORE_PRINTLen = 0 Then Exit Sub
    OreLCD_RS 1

    'Write Data
    dim i as integer
    For i = 1 To ORE_PRINTLen step 1
      #ifdef OreDebug 1
        HSerPrint "String Index "
        HSerPrint i
        HSerPrintCRLF   
        HSerPrint "Char "
        HSerPrint ORE_PRINTData(i)
        HSerPrintCRLF   
      #endif
        ORE_LCDWriteByte Ore_PRINTData(i)
    Next
    OreLCD_RS 0
End Sub

; OBSO
; Ore_LCDHex で代用
; Ore_Print は、あくまで、与えた文字列や整数をそのまま描画することにして、
; バイトコード送るのは Ore_LCDHex の役目にする
;SUB ORE_PRINT (In OreLCDValue)
;SUB ORE_PRINT (In OreLCDValue as Byte) ; と書くべきか？
;'Sub to PRINT a byte variable on the LCD
;
;    OreLCDValueTemp = 0
;    ;Set LCD_RS On
;    OreLCD_RS(1)
;
;    IF OreLCDValue >= 100 Then
;        OreLCDValueTemp = OreLCDValue / 100
;        OreLCDValue = SysCalcTempX
;        ORE_LCDWriteByte(OreLCDValueTemp + 48)
;    End If
;
;    If OreLCDValueTemp > 0 Or OreLCDValue >= 10 Then
;        OreLCDValueTemp = OreLCDValue / 10
;        OreLCDValue = SysCalcTempX
;        ORE_LCDWriteByte(OreLCDValueTemp + 48)
;    End If
;    ORE_LCDWriteByte (OreLCDValue + 48)
;    OreLCD_RS 0
;End Sub

; なんか間違ってない？
; Ore_Print WordToString(65536) ; とか
; Ore_Print [long]65536 ; とか、そんな感じで良いんじゃね？
;SUB ORE_PRINT (In LCDValue As Word)
;'Sub to PRINT a word variable on the LCD
;
;    Dim SysCalcTempX As Word
;
;    OreLCD_RS(1)
;    LCDValueTemp = 0
;
;    If LCDValue >= 10000 then
;        LCDValueTemp = LCDValue / 10000 [word]
;         
;         ; dim で宣言 = 0 になるとすると LCDValue = 0になっちゃうよ
;         ; これ 10000 で割った余りじゃないといけないんじゃないの？
;         ; LCDValue = LCDValue % 10000 ; とかじゃないの？？？
;        LCDValue = SysCalcTempX  
;        ORE_LCDWriteByte(LCDValueTemp + 48)
;        Goto LCDPRINTWord1000
;    End If
;
;    If LCDValue >= 1000 then
;        LCDPRINTWord1000:
;        LCDValueTemp = LCDValue / 1000 [word]
;        LCDValue = SysCalcTempX
;        ORE_LCDWriteByte(LCDValueTemp + 48)
;        Goto LCDPRINTWord100
;    End If
;
;    If LCDValue >= 100 then
;        LCDPRINTWord100:
;        LCDValueTemp = LCDValue / 100 [word]
;        LCDValue = SysCalcTempX
;        ORE_LCDWriteByte(LCDValueTemp + 48)
;        Goto LCDPRINTWord10
;    End If
;
;    If LCDValue >= 10 then
;        LCDPRINTWord10:
;        LCDValueTemp = LCDValue / 10 [word]
;        LCDValue = SysCalcTempX
;        ORE_LCDWriteByte(LCDValueTemp + 48)
;    End If
;
;    ORE_LCDWriteByte (LCDValue + 48)
;    OreLCD_RS 0
;End Sub

; obso
; 最後に Ore_Print word に送るんだけど、その word が動作する気がしない
;SUB ORE_PRINT (In ORE_LCDValueInt As Integer)
;'Sub to PRINT an integer variable on the LCD
;
;    Dim LCDValue As Word
;
;    'If sign bit is on, PRINT - sign and then negate
;    If ORE_LCDValueInt.15 = On Then
;              ;LCDWriteChar("-")
;              ore_print "-"
;              LCDValue = -ORE_LCDValueInt
;
;    'Sign bit off, so just copy value
;    Else
;              LCDValue = ORE_LCDValueInt
;    End If
;
;    'Use PRINT(word) to display value
;    ORE_PRINT LCDValue
;End Sub

; LongToString ですかっと送れば良い気もするけどね
SUB ORE_PRINT (In OreLCDValue As Long)
'Sub to PRINT a long variable on the LCD
    #ifdef OreDebug 1
      HSerPrint "LCDValue "
      HSerPrint OreLCDValue
      HSerPrintCRLF   
    #endif

    Dim OreSysPrintBuffer(10)
    Dim ore_len as integer
    ore_len=0

    Do
        'Divide number by 10, remainder into buffer
        ore_len = ore_len+1
        #ifdef OreDebug 1
          HSerPrint "ore_len "
          HSerPrint ore_len
          HSerPrintCRLF   
        #endif
        OreSysPrintBuffer(ore_len) = OreLCDValue % 10
        #ifdef OreDebug 1
          HSerPrint "OreSysPrintBuffer(ore_len) "
          HSerPrint OreSysPrintBuffer(ore_len)
          HSerPrintCRLF   
        #endif
        OreLCDValue = (OreLCDValue-OreSysPrintBuffer(ore_len))/10
        #ifdef OreDebug 1
          HSerPrint "OreLCDValue "
          HSerPrint OreLCDValue
          HSerPrintCRLF   
        #endif
    Loop While OreLCDValue <> 0

    'Display
    OreLCD_RS(1)
    dim i as integer
    For i = ore_len To 1 Step -1
      #ifdef OreDebug 1
        ;HSerPrint "i "
        ;HSerPrint i
        ;HSerPrintCRLF   
        ;HSerPrint "OreSysPrintBuffer() "
        ;HSerPrint OreSysPrintBuffer(i)
        ;HSerPrintCRLF   
      #endif
        ORE_LCDWriteByte(OreSysPrintBuffer(i) + 48) ;48 は文字コード表で "0" の場所
    Next

    OreLCD_RS(0)
End Sub


sub Ore_LCDHex  (In Ore_LCDValue as byte)
' 文字コードを直接叩く

    OreLCD_RS(1)
    ORE_LCDWriteByte(Ore_LCDValue)
    OreLCD_RS(0)

end sub

sub ORE_LCDWriteByte(In Ore_LCDByte )

            OreLCD_DB7(Ore_LCDByte.7)
            OreLCD_DB6(Ore_LCDByte.6)
            OreLCD_DB5(Ore_LCDByte.5)
            OreLCD_DB4(Ore_LCDByte.4)

        Wait 1 us
        OreLCD_EN_PULSE

        ; 下 4ビットを送る

            OreLCD_DB7(Ore_LCDByte.3)
            OreLCD_DB6(Ore_LCDByte.2)
            OreLCD_DB5(Ore_LCDByte.1)
            OreLCD_DB4(Ore_LCDByte.0)

        Wait 1 us
        OreLCD_EN_PULSE

; LCD_State がどこにも使われていない (少なくとも採用した sub の中では)
; 要らないと思う
;    'If Register Select is low
;    IF OreLCD_RS_STATE = 0 then
;        IF LCDByte < 16 then
;            if LCDByte > 7 then
;               LCD_State = LCDByte
;            end if
;        END IF
;    END IF

end sub

;sub LCDWriteChar(In LCDChar)
;'Sub to PRINT character on the LCD
;
;     set LCD_RS on
;     LCDWriteByte(LCDChar)
;     wait 5 10us
;end sub
;
;function LCDReady
;
;    #IFDEF LCD_NO_RW
;        Wait 10 ms
;        LCDReady = TRUE
;        exit function
;    #ENDIF
;
;    #IFNDEF LCD_NO_RW
;        #IFDEF LCD_IO 1, 2, 2_74XX174, 2_74XX164  ' All 1 and 2-wire modes
;            LCDReady = TRUE
;            exit function
;        #ENDIF
;    #ENDIF
;
;end function

;Sub CheckBusyFlag
;'Sub that waits until LCD controller busy flag goes low (ready)
;'Only used by LCD_IO 4,8 and only when LCD_NO_RW is NOT Defined
;'Called by sub LCDNOrmalWriteByte
;    #IFDEF LCD_IO 4,8
;
;        #IFNDEF LCD_NO_RW
;
;             LCD_RSTemp = LCD_RS
;             DIR SCRIPT_LCD_BF  IN
;             SET LCD_RS OFF
;             SET LCD_RW ON
;
;            #IFDEF LCD_IO 4
;                LCDTEMPRWCount = 0
;                Do
;                    wait 1 us
;                    Set LCD_Enable ON
;                    wait 1 us
;                    SysLCDTemp.7 = SCRIPT_LCD_BF
;                    Set LCD_Enable OFF
;                    Wait 1 us
;                    PulseOut LCD_Enable, 1 us
;                    Wait 1 us
;                    if LCDTEMPRWCount = 255 Then SysLCDTemp.7 = 0
;                    LCDTEMPRWCount++
;                Loop While SysLCDTemp.7 <> 0  
;            #ENDIF
;
;
;            #IFDEF LCD_IO 8
;                LCDTEMPRWCount = 0
;                Do
;                   Wait 1 us
;                   Set LCD_Enable ON
;                   Wait 1 us
;                   SysLCDTemp.7 = SCRIPT_LCD_BF
;                   Set LCD_Enable OFF
;                    if LCDTEMPRWCount = 255 Then SysLCDTemp.7 = 0
;                    LCDTEMPRWCount++
;
;                Loop While SysLCDTemp.7 <> 0
;
;            #ENDIF
;
;            LCD_RS = LCD_RSTemp
;
;        #ENDIF
;
;    #ENDIF
;
;End Sub


;SUB LCD2_NIBBLEOUT (In LCD2BYTE)
;'Sub to send byte in two nibbles to LCD
;    #IFDEF LCD_IO 1
;        '1-wire mode with 74HC595
;
;        'Idle high
;        SET LCD_CD ON
;
;        'Send QH/QH' as a "One"; mandatory for resetting the shiftreg and a working monoflop
;        Onebit
;
;        IF LCD_RS ON THEN
;            'Send QG; LCD RS signal
;            Onebit
;        ELSE
;            Zerobit
;        END IF
;
;        'Send QF; Spare pin
;        Zerobit
;
;        If LCD_Backlight Then ' Background LED is used
;            'Send QE (pin 4 74HC595); used for Backlight
;            Onebit
;        Else
;            Zerobit
;        End if
;
;        REPEAT 4
;            'Send QD - QA to Shift register; starting from Nibble most significant bit
;            IF LCD2Byte.3 ON THEN
;                Onebit
;            Else
;                Zerobit
;            END IF
;            ROTATE LCD2Byte LEFT
;        END REPEAT
;
;      'Generate reset signal for shiftreg and activate monoflop
;      ResetShiftReg
;
;        Wait LCD_SPEED  us
;
;    #ENDIF
;
;    #IFDEF LCD_IO 2, 2_74XX174, 2_74XX164
;        '2-wire modes with Shiftreg
;
;        ' Set Data and Clock bits off
;        SET LCD_DB OFF
;        SET LCD_CB OFF
;
;        #IFDEF LCD_IO 2, 2_74XX174
;            'This is mode with 74LS174 or  74HC164 with diode connected to pin 11
;            'Backilight not supported
;
;            'Clear Shift Register With six zero's
;            REPEAT 6
;                'STROBE
;                SET LCD_CB ON
;                SET LCD_CB OFF
;            END REPEAT
;        #ENDIF
;
;         #IFDEF LCD_IO 2_74XX164
;            'This is mode with 74HC164 and Backlight
;
;            'Clear Shift Register with eight zero's
;            REPEAT 8
;                ' STROBE
;                SET LCD_CB ON
;                SET LCD_CB OFF
;            END REPEAT
;        #ENDIF
;
;        'First bit out to Shift register, always 1 for E gate LCD
;        SET LCD_DB ON
;        'STROBE
;        SET LCD_CB ON
;        SET LCD_CB OFF
;
;
;        #IFDEF LCD_IO 2_74XX164
;            'Set Spare pin and background pin 74HC164
;
;            'Spare bit (QG)
;            SET LCD_DB OFF
;            'STROBE
;            SET LCD_CB ON
;            SET LCD_CB OFF
;
;            ' Background LED is used
;            If LCD_Backlight Then
;                'send a one bit (QF, pin 11 74HC164)
;                SET LCD_DB ON
;                'STROBE
;                SET LCD_CB ON
;                SET LCD_CB OFF
;            Else
;                'send a zero bit (QF, pin 11 74HC164)
;                SET LCD_DB OFF
;                'STROBE
;                SET LCD_CB ON
;                SET LCD_CB OFF
;            End if
;        #ENDIF
;
;      SET LCD_DB ON
;      'Shift register (QE), R/S gate LCD
;      IF LCD_RS OFF THEN SET LCD_DB OFF
;      'Strobe
;      SET LCD_CB ON
;      SET LCD_CB OFF
;
;        '4 bits Data (QD - QA) out to Shift register, starting from Nibble most significant bit
;        'FOR LCDTemp = 1 to 4
;        REPEAT 4
;            SET LCD_DB OFF
;            IF LCD2Byte.3 ON THEN SET LCD_DB ON
;            'STROBE
;            SET LCD_CB ON
;            SET LCD_CB OFF
;            ROTATE LCD2Byte LEFT
;        END REPEAT
;
;        ' Last pulse for Nibble output. Not for Shift register
;        SET LCD_DB ON
;        ' Put E pin on LCD to one through an AND operation
;        WAIT 1 MS
;        ' with the first bit outputted (QH)
;        SET LCD_DB OFF
;    #ENDIF
;END SUB

'LCDNormalReadByte Only supported in LCD_IO 4,8
;function LCDNormalReadByte
;
;    #IFDEF LCD_NO_RW
;        LCDReadByte = 0
;        Exit Function
;    #ENDIF
;
;    #IFNDEF LCD_NO_RW
;
;        set LCD_RW ON 'Read mode
;        LCDReadByte = 0
;
;        #IFDEF LCD_IO 4
;            'Set pins to input
;            DIR LCD_DB4 IN
;            DIR LCD_DB5 IN
;            DIR LCD_DB6 IN
;            DIR LCD_DB7 IN
;
;            'Read upper nibble from input pins
;            SET LCD_Enable ON
;            Wait LCD_Write_Delay     '2 us
;            if LCD_DB7 ON then SET LCDReadByte.7 ON
;            if LCD_DB6 ON THEN SET LCDReadByte.6 ON
;            if LCD_DB5 ON then SET LCDReadByte.5 ON
;            if LCD_DB4 ON THEN SET LCDReadByte.4 ON
;            SET LCD_Enable OFF
;            Wait 5 us
;
;            'Read lower nibble from input pins
;            SET LCD_Enable ON
;            Wait LCD_Write_Delay
;            if LCD_DB7 ON then SET LCDReadByte.3 ON
;            if LCD_DB6 ON THEN SET LCDReadByte.2 ON
;            if LCD_DB5 ON then SET LCDReadByte.1 ON
;            if LCD_DB4 ON THEN SET LCDReadByte.0 ON
;            SET LCD_Enable OFF
;            Wait 5 us
;        #ENDIF
;
;        #IFDEF LCD_IO 8
;            DIR LCD_DATA_PORT 255
;            SET LCD_Enable ON
;            Wait LCD_Write_Delay
;            LCDReadByte = LCD_DATA_PORT
;            SET LCD_Enable OFF
;            Wait 5 us
;         #ENDIF
;    #ENDIF
;end function

' Method uses parameters LCDON, LCDOFF, CURSORON, CURSOROFF, FLASHON, Or FLASHOFF
;sub LCDCursor(In LCDCRSR)
;'Sub  to set cursor style
;    Set LCD_RS OFF
;
;    If LCDCRSR = ON  Then LCDTemp = LCD_State OR LCDON
;    IF LCDCRSR = LCDON Then LCDTemp = LCD_State OR LCDON
;
;    If LCDCRSR = OFF Then LCDTemp = LCD_State AND LCDOFF
;    If LCDCRSR = LCDOFF Then LCDTemp = LCD_State AND LCDOFF
;
;    If LCDCRSR = CursorOn Then LCDTemp = LCD_State OR CursorON
;    If LCDCRSR = CursorOFF then LCDTemp = LCD_State and CursorOFF
;
;    If LCDCRSR = FLASH  Then LCDTemp = LCD_State OR FLASHON
;    If LCDCRSR = FLASHON  Then LCDTemp = LCD_State OR FLASHON
;    If LCDCRSR = FLASHOFF then LCDTemp = LCD_State and FLASHOFF
;
;    LCDWriteByte(LCDTemp)
;    'save last state
;    LCD_State = LCDtemp
;
;end sub

;sub LCDCreateChar (In LCDCharLoc, LCDCharData())
;'Sub a create a custom character in CGRAM
;
;    'Store old location
;      #IFDEF LCD_IO 4,8,10,12
;        #ifndef LCD_NO_RW
;          Set LCD_RS Off
;          LCDLoc = LCDReadByte
;          Set LCDLoc.7 On
;        #endif
;    #ENDIF
;
;    'Select location
;    Set LCD_RS Off
;    LCDWriteByte (64 + LCDCharLoc * 8)
;    wait 5 10us
;
;    'Write char
;    Set LCD_RS On
;    For LCDTemp = 1 to 8
;        LCDWriteByte LCDCharData(LCDTemp)
;        wait 5 10us
;    Next
;
;    'Restore location
;    #IFDEF LCD_IO 4,8,10,12
;        #ifndef LCD_NO_RW
;            Set LCD_RS Off
;            LCDWriteByte (LCDLoc)
;            wait 5 10us
;        #endif
;
;        #ifdef LCD_NO_RW
;            set LCD_RS off
;            LCDWriteByte(0x80)
;            wait 5 10us
;        #endif
;    #endif
;
;    #ifndef LCD_IO 4,8,10,12
;       set LCD_RS off
;       LCDWriteByte(0x80)
;       wait 5 10us
;     #endif
;end sub

;Sub LCDCreateGraph(In LCDCharLoc, In LCDValue)
;'Sub to create a graph character in CGRAM
;
;    'Store old location
;    #IFDEF LCD_IO 4,8
;        #ifndef LCD_NO_RW
;          Set LCD_RS Off
;          LCDLoc = LCDReadByte
;          Set LCDLoc.7 On
;        #endif
;    #ENDIF
;
;    'Select location
;    Set LCD_RS Off
;    LCDWriteByte (64 + LCDCharLoc * 8)
;    wait 5 10us
;
;    'Write char for graph
;    Set LCD_RS On
;    For LCDTemp = 8 to 1
;        If LCDTemp > LCDValue Then
;            LCDWriteByte LCD_BAR_EMPTY
;        Else
;            LCDWriteByte LCD_BAR_FULL
;        End If
;        wait 5 10us
;    Next
;
;    'Restore location
;      #IFDEF LCD_IO 4,8
;        #ifndef LCD_NO_RW
;            Set LCD_RS Off
;            LCDWriteByte (LCDLoc)
;            wait 5 10us
;        #endif
;
;        #ifdef LCD_NO_RW
;            set LCD_RS off
;            LCDWriteByte(0x80)
;            wait 5 10us
;        #endif
;    #endif
;
;    #ifndef LCD_IO 4,8
;        set LCD_RS off
;        LCDWriteByte(0x80)
;        Wait 5 10us
;    #endif
;End Sub

;sub LCDSpace(in LCDValue)
;'Sub to PRINT a number of spaces - upto 40
;
;    set LCD_RS on
;    if LCDValue > 40 then LCDValue = 40
;    do until LCDValue = 0
;        LCDWriteByte(32)
;        LCDValue --
;    loop
;end sub

;sub LCDDisplayOff
;'Sub to turn off display and turn off cursor and turn off flash
;
;    set LCD_RS off
;    LCDWriteByte(0b00001000)
;    LCD_State = 0
;    wait 10 ms
;end sub

;sub LCDDisplayOn
;'Sub to turn ON display, turn off curor and turn off flash
;
; set LCD_RS off
;    LCDWriteByte(0b00001100)
;    LCD_State = 8
;    wait 10 ms
;End Sub
;
;sub LCDBacklight(IN LCDTemp)
;'Sub set to backlight state
;      #IFDEF LCD_IO 1, 2_74XX164      '1 and 2-wire mode with Backlight
;        IF LCDTemp = ON then
;          LCD_Backlight = LCD_Backlight_On_State
;        Else
;          LCD_Backlight = LCD_Backlight_Off_State
;        END IF
;        Set LCD_RS OFF
;        LCDWriteByte(0)
;    #ENDIF
;
;    #IFDEF LCD_IO 0,4,8, 404
;        'Set the port for this mode
;        IF LCDTemp = OFF then set LCD_Backlight LCD_Backlight_Off_State
;        IF LCDTemp = ON then  set LCD_Backlight LCD_Backlight_On_State
;    #ENDIF
;
;    #IFDEF LCD_IO 10,12
;        'Assign the variable for this mode
;        IF LCDTemp = OFF then LCD_Backlight = LCD_Backlight_Off_State
;        IF LCDTemp = ON then  LCD_Backlight = LCD_Backlight_On_State
;        ' write a zero and the method will set display backlite
;        Set LCD_RS OFF
;        LCDWriteByte(0)
;    #ENDIF
;end Sub

;Dim SysLCDTemp as Byte

;Sub PUT (In LCDPutLine, In LCDPutColumn, In LCDChar)
;'Sub to put a character at the specified location
;    LOCATE LCDPutLine, LCDPutColumn
;    Set LCD_RS on
;    LCDWriteByte(LCDChar)
;End Sub

;Function GET (LCDPutLine, LCDPutColumn)
;'Sub to get the value of the current LCD GCGRAM
;'GET only supported in 8 and 4 bit modes
;
;    LOCATE LCDPutLine, LCDPutColumn
;    Set LCD_RS on
;    GET = LCDReadByte
;End Function

;Sub LCDHOME
; Ore_Locate でカバーできる
;'Sub to set the cursor to the home position
;    SET LCD_RS OFF
;    'Return CURSOR to home
;    LCDWriteByte (0b00000010)
;    Wait 2 ms 'Must be > 1.52 ms
;End Sub

;Sub LCDcmd ( In LCDValue )
;'Sub to send specified command direct to the LCD
;    SET LCD_RS OFF
;
;    LCDWriteByte ( LCDValue)
;
;    IF LCDValue = 1 OR LCDValue = 2 then  ' HOME or CLEAR
;       Wait 2 ms ' Must be > 1.52 ms
;    Else
;       Wait 50 us
;    END IF
;
;End sub

