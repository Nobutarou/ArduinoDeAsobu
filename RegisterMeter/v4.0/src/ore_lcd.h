; オリジナルは /opt/GCBASIC/include/lowlevel/lcd.h
; include してないのに勝手に読みこまれて、関数の型が違うとか文句を行ってくるから、
; 別のフォルダに移しておいた
;
; I2C 接続の LCD_IO 10 は I2C で 7ビットを送ると対応するピンがそのまま High, Low になっ
; て動く。多分 IOエキスパンダの GPIOA だけで完結してるんだと思う。
; こっちは、無理矢理やってるから GPIOA, GPIOB 両方使うので、ピン配置を変えるだけでは対応できない。
; 普通の 4線通信の LCD_IO 4 を変更する

; あまりに汚いコード。いつか書き直す実力が備わると良いのだが。

; print, locate,cls を動かすことが目標。
; それらに関係ない sub は無視

; 初期設定
; RS = B0 (PIC ではなくて GPIO で記述)
; RW = B5
; EN = B4
; D4 = B2
; D5 = B3
; D6 = B1
; D7 = A2

dim RS_STATE as bit

; なぜか 1 を渡しても 0 が渡ってしまう
;macro RS(bit)
;  OUT_GPB_BIT(DeviceAddress,0,bit)
;  ;HSerPrint "RS "
;  ;HSerPrint bit
;  ;HSerPrintCRLF   
;
;  RS_STATE=bit
;end macro

; コピーして名前を変えただけなのになぜか動く
macro ORE_RS(bit)
  OUT_GPB_BIT(DeviceAddress,0,bit)
  ;HSerPrint "RS "
  ;HSerPrint bit
  ;HSerPrintCRLF   

  RS_STATE=bit
end macro

macro RW(bit)
  OUT_GPB_BIT(DeviceAddress,5,bit)
  ;HSerPrint "RW "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

;macro EN(bit)
;  OUT_GPB_BIT(DeviceAddress,4,bit)
;  ;HSerPrint "EN "
;  ;HSerPrint bit
;  ;HSerPrintCRLF   
;end macro
macro ORE_EN(bit)
  OUT_GPB_BIT(DeviceAddress,4,bit)
  ;HSerPrint "EN "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

; これらの時間は 270kHz 動作時の話。100kHz ではおよそ 3倍となる。
; イネーブルセットアップ時間 tAS は最小で 140ns。
; イネーブルパルス幅は最小で 450ns。
; イネーブルサイクル時間は最小で 1000ns。
; だがインストラクション実行時間が基本 37us。
; 37us を越えるクリアとカーソルホームは別途 wait を掛けよう

macro EN_PULSE
  wait 1 us ; 1 us >> 140 ns *3
  ORE_EN 1
  wait 2 us ; 2 us >> 450 ns *3
  ORE_EN 0
  wait 220 us ; 220us >> 37 us * 3
end macro

macro DB4(bit)
  OUT_GPB_BIT(DeviceAddress,2,bit)
  ;HSerPrint "DB4 "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

macro DB5(bit)
  OUT_GPB_BIT(DeviceAddress,3,bit)
  ;HSerPrint "DB5 "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

macro DB6(bit)
  OUT_GPB_BIT(DeviceAddress,1,bit)
  ;HSerPrint "DB6 "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

macro DB7(bit)
  OUT_GPA_BIT(DeviceAddress,2,bit)
  ;HSerPrint "DB7 "
  ;HSerPrint bit
  ;HSerPrintCRLF   
end macro

#define ORE_LCD_WIDTH 16

Dim SysLCDTemp as Byte

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

Sub ORE_LOCATE (In LCDLine, In LCDColumn)
'Sub to LOCATE the cursor
'Where LCDColumn is 0 to screen width-1, LCDLine is 0 to screen height-1

    If LCDLine > 1 Then
        LCDLine = LCDLine - 2
        LCDColumn = LCDColumn + ORE_LCD_WIDTH
    End If

    ; 0x80 = 0b1000000, or だから先頭の 1 は必ず残る。これは DD RAM アドレスセットのインス
    ; トラクションの合図
    ; 0x40 = 0b0100000。先頭行のアドレスが 0、二行目が 0x40 から始まる。LCDLINE=1 となれば
    ; 二行目となる
    ORE_LCDWriteByte(0x80 or 0x40 * LCDLine + LCDColumn)
End Sub

Sub ORE_CLS
'Sub to clear the LCD
    ORE_RS(0)

    'Clear screen
    ORE_LCDWriteByte (0b00000001)
    Wait 4700 us ;1.52ms x3

    ' LCD の左上に文字を出す準備を始めたみたいな感じ
    ; 0x80 = 0b10000000
    ORE_LCDWriteByte(0x80)
    Wait 220 us

End Sub

;Sub LCDHOME
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

;Sub LCD3_CMD(In LCDValue as Byte)
;    LCD_DB = 0  'really maps to the LCD_RS
;    LCDWriteByte(LCDValue)
;end sub
;
;sub LCD3_DATA(In LCDValue as byte)
;    LCD3_DB = 1  'really maps to the LCD_RS
;    LCDWriteByte(LCDValue)
;end sub

;' Used in 1-wire mode; a "zero" bit is 10us low and minimal 20 us High
;Sub Zerobit
;    SET LCD_CD OFF
;    wait 10 us ' bit time
;    SET LCD_CD ON
;    wait 20 us ' recovery RC time
;    wait 1 ms
;end sub
;
;' Used in 1-wire mode; a "one" bit is 1us low and minimal 5 us High
;Sub Onebit
;    SET LCD_CD OFF
;    wait 1 us ' bit time
;    SET LCD_CD ON
;    wait 5 us ' recovery RC time
;    wait 1 ms
;end sub
;
;' Used in 1-wire mode; reset is 350 us low and minimal 300 us high
;Sub ResetShiftReg
;    SET LCD_CD OFF
;    wait 350 us
;    SET LCD_CD ON
;    wait 300 us
;    wait 1 ms
;end sub

sub InitLCD
; データシートと読み比べていく

    asm showdebug  `LCD_IO selected is ` LCD_IO

    #IFDEF LCD_Speed FAST
        asm showdebug  `LCD_Speed is FAST`
    #ENDIF
    #IFDEF LCD_Speed MEDIUM
        asm showdebug  `LCD_Speed is MEDIUM`
    #ENDIF
    #IFDEF LCD_Speed SLOW
        asm showdebug  `LCD_Speed is SLOW`
    #ENDIF

    asm showdebug  `OPTIMAL is set to ` OPTIMAL
    asm showdebug  `LCD_Speed is set to ` LCD_Speed


        ; VDD が 4.5V に到達してから 15ms 以上待つ
        ; 270kHz の話と仮定して 100kHz であれば 
        Wait 47 ms

        ; ORE_RS| R/W | DB7-DB4 | で
        ; 0 | 0   | 0011    | 

        ORE_RS 0
        RW 0
        ORE_EN 0

        DB7 0
        DB6 0
        DB5 1
        DB4 1

        EN_PULSE

        ; 4.1ms 以上待つ (x3)
        Wait 22 ms
        
        ; 2回目
        EN_PULSE
        
        ; 100us 以上待つ (x3)
        wait 470 us

        ; 3回目
        EN_PULSE

        ; RS| R/W | DB7-DB4 | 
        ; 0 | 0   | 0010    | 4 bit mode の指定

        DB7 0
        DB6 0
        DB5 1
        DB4 0
        EN_PULSE

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
    LCD_State = 12

end sub


SUB ORE_PRINT (In ORE_PRINTData As String)
'Sub to PRINT a string variable on the LCD
    ;HSerPrint "PRINTData "
    ;HSerPrint ORE_PRINTData
    ;HSerPrintCRLF   

    Ore_PRINTLen = ORE_PRINTData(0)
    ;HSerPrint "PRINTLen "
    ;HSerPrint ORE_PRINTLen
    ;HSerPrintCRLF   

    If ORE_PRINTLen = 0 Then Exit Sub
    ;Set LCD_RS On
    ORE_RS 1
    ;HSerPrint "oreprint orers 1"
    ;HSerPrintCRLF   
    ;do
    ;loop
    'Write Data
    dim i as integer
    ;For SysPRINTTemp = 1 To PRINTLen
    For i = 1 To ORE_PRINTLen step 1
        ;HSerPrint "SysPRINTTemp "
        ;HSerPrint i
        ;HSerPrintCRLF   
        ;HSerPrint "PRINTDATA(SysPRINTTemp) "
        ;HSerPrint ORE_PRINTData(SysPRINTTemp)
        ;HSerPrintCRLF   
        ORE_LCDWriteByte Ore_PRINTData(i)
    Next
    ORE_RS 0
End Sub

SUB ORE_PRINT (In LCDValue)
'Sub to PRINT a byte variable on the LCD

    LCDValueTemp = 0
    ;Set LCD_RS On
    ORE_RS(1)

    IF LCDValue >= 100 Then
        LCDValueTemp = LCDValue / 100
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
    End If

    If LCDValueTemp > 0 Or LCDValue >= 10 Then
        LCDValueTemp = LCDValue / 10
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
    End If
    ORE_LCDWriteByte (LCDValue + 48)
    ORE_RS 0
End Sub

SUB ORE_PRINT (In LCDValue As Word)
'Sub to PRINT a word variable on the LCD

    Dim SysCalcTempX As Word

    ;Set LCD_RS On
    ORE_RS(1)
    LCDValueTemp = 0

    If LCDValue >= 10000 then
        LCDValueTemp = LCDValue / 10000 [word]
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
        Goto LCDPRINTWord1000
    End If

    If LCDValue >= 1000 then
        LCDPRINTWord1000:
        LCDValueTemp = LCDValue / 1000 [word]
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
        Goto LCDPRINTWord100
    End If

    If LCDValue >= 100 then
        LCDPRINTWord100:
        LCDValueTemp = LCDValue / 100 [word]
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
        Goto LCDPRINTWord10
    End If

    If LCDValue >= 10 then
        LCDPRINTWord10:
        LCDValueTemp = LCDValue / 10 [word]
        LCDValue = SysCalcTempX
        ORE_LCDWriteByte(LCDValueTemp + 48)
    End If

    ORE_LCDWriteByte (LCDValue + 48)
    ORE_RS 0
End Sub

SUB ORE_PRINT (In ORE_LCDValueInt As Integer)
'Sub to PRINT an integer variable on the LCD

    Dim LCDValue As Word

    'If sign bit is on, PRINT - sign and then negate
    If ORE_LCDValueInt.15 = On Then
              ;LCDWriteChar("-")
              ore_print "-"
              LCDValue = -ORE_LCDValueInt

    'Sign bit off, so just copy value
    Else
              LCDValue = ORE_LCDValueInt
    End If

    'Use PRINT(word) to display value
    ORE_PRINT LCDValue
End Sub

SUB ORE_PRINT (In OreLCDValue As Long)
'Sub to PRINT a long variable on the LCD
    ;HSerPrint "LCDValue "
    ;HSerPrint OreLCDValue
    ;HSerPrintCRLF   

    ;Dim SysCalcTempA As Long
    Dim SysPRINTBuffer(10)
    Dim ore_len as integer
    ;SysPRINTBuffLen = 0
    ore_len=0

    Do
        'Divide number by 10, remainder into buffer
        ;SysPRINTBuffLen += 1
        ore_len = ore_len+1
        ;HSerPrint "SysPRINTBuffLen "
        ;HSerPrint ore_len
        ;HSerPrintCRLF   
        ;SysPRINTBuffer(SysPRINTBuffLen) = OreLCDValue % 10
        SysPRINTBuffer(ore_len) = OreLCDValue % 10
        ;HSerPrint "SysPRINTBuff() "
        ;HSerPrint SysPRINTBuffer(ore_len)
        ;HSerPrintCRLF   
        ;OreLCDValue = SysCalcTempA
        OreLCDValue = (OreLCDValue-SysPRINTBuffer(ore_len))/10
        ;HSerPrint "OreLCDValue "
        ;HSerPrint OreLCDValue
        ;HSerPrintCRLF   
    Loop While OreLCDValue <> 0

    'Display
    ;Set LCD_RS On
    ORE_RS(1)
    dim i as integer
    ;For SysPRINTTemp = SysPRINTBuffLen To 1 Step -1
    For i = ore_len To 1 Step -1
        ;HSerPrint "i "
        ;HSerPrint i
        ;HSerPrintCRLF   
        ;HSerPrint "SysPRINTBuffer() "
        ;HSerPrint SysPRINTBuffer(i)
        ;HSerPrintCRLF   
              ORE_LCDWriteByte(SysPRINTBuffer(i) + 48)
    Next

    ORE_RS(0)
End Sub


sub Ore_LCDHex  (In Ore_LCDValue as byte)
' 文字コードを直接叩く

    ORE_RS(1)
    ORE_LCDWriteByte(Ore_LCDValue)
    ORE_RS(0)

end sub
;
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


sub ORE_LCDWriteByte(In LCDByte )


            DB7(LCDByte.7)
            DB6(LCDByte.6)
            DB5(LCDByte.5)
            DB4(LCDByte.4)

        Wait 1 us
        EN_PULSE

        ; 下 4ビットを送る

            DB7(LCDByte.3)
            DB6(LCDByte.2)
            DB5(LCDByte.1)
            DB4(LCDByte.0)

        Wait 1 us
        EN_PULSE

    'If Register Select is low
    IF RS_STATE = 0 then
        IF LCDByte < 16 then
            if LCDByte > 7 then
               LCD_State = LCDByte
            end if
        END IF
    END IF

end sub

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

