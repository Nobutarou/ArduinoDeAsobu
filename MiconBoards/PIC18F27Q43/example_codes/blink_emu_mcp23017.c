; chip_type, clock (MHz)
; クロックが低いと I2C 失敗が多い？
; 4MHz まず動かない
; 8MHz まず動くみたい
; 16MHz ここまで上げたら安心か？
#Chip 18f27q43,16

; The default is LVP = OFF. Very dangerous.
#config LVP = ON
#config MCLR = ON

; 自作 mcp23017 は A0-A2, C0-C5 が GPIO A, B として使える

; PPS。SCL を RC4, SDA を RC5 にしておこう
; 双方向なので input, output 両方必要
I2C1SCLPPS = 0b010100;
RC4PPS = 0x37
I2C1SDAPPS = 0b010101;
RC5PPS = 0x38

; I2C の設定も別途やっておかないとだめなようだ
; mcp23017.h が勝手にやってくれたりはしない
HI2CMODE  Master
#define HI2C_DATA PORTC.5
#define HI2C_CLOCK PORTC.4
; default 記述が 100 だったり 125 だったり怪しい。データシート的には 100kHz がデフォルト
#define HI2C_BAUD_RATE 100
Dir HI2C_DATA out
Dir HI2C_CLOCK out 

#include <mcp23017.h>

; 7bit で 0x20=0b100000 だけど 1ビットシフトさせるから 0b1000000 = 0x40 となる。 
#define DeviceAddress 0x40 

; 修正が必要な Sub 達
Sub MCP23017_ReadByte (In MCP23017_device, In MCP23017_instr , Out MCP23017_data)
           ;HI2reCStart ; typo。修正済の模様
           HI2CRestart
           HI2CSend( MCP23017_device )
           HI2CSend( MCP23017_instr )
           HI2CRestart
           HI2CSend( MCP23017_device + 1)
           HI2CReceive( MCP23017_data , NACK )
           HI2CStop
End Sub


; STATE_BIT を bit 型だと、コンパイラに教えないとエラーになる
; ここで議論されている
; https://sourceforge.net/p/gcbasic/discussion/579125/thread/223185c934/
; すでに修正されている
; https://sourceforge.net/p/gcbasic/code/HEAD/tree/GCBASIC/trunk/include/MCP23017.h
Sub OUT_GPA_BIT (In DeviceAddress, In BIT_A, In State_Bit)
        DataGpAByte = IN_GPA_BYTE (DeviceAddress)
        If State_Bit Then
           DataGpAByte.BIT_A = 1
        Else
           DataGpAByte.BIT_A = 0
        End If
        OUT_GPA_BYTE (DeviceAddress, DataGpAByte)
End Sub

Sub OUT_GPB_BIT (In DeviceAddress, In Bit_B, In State_Bit)
        DataGpBByte = IN_GPB_BYTE (DeviceAddress)
        If State_Bit Then
           DataGpBByte.BIT_B = 1
        Else
           DataGpBByte.BIT_B = 0
        End If
        OUT_GPB_BYTE(DeviceAddress, DataGpBByte)
End Sub

INIT_MCP23017_GPA DeviceAddress, 0 ; Aポートは output
INIT_MCP23017_GPB DeviceAddress, 0 ; Bポートは output

; do
;   OUT_GPB_BIT(DeviceAddress, 5, 1) ; 
;   wait 1 s
;   OUT_GPB_BIT(DeviceAddress, 5, 0) ; 
;   wait 1 s
; loop

Do
  OUT_GPB_BIT DeviceAddress, 5, 0 ; B5 off
  OUT_GPA_BIT(DeviceAddress, 0, 1) ; A0 on
  wait 100 ms
  OUT_GPA_BIT(DeviceAddress, 0, 0) ; 
  OUT_GPA_BIT(DeviceAddress, 1, 1) ; 
  wait 100 ms
  OUT_GPA_BIT(DeviceAddress, 1, 0) ; 
  OUT_GPA_BIT(DeviceAddress, 2, 1) ; 
  wait 100 ms
  OUT_GPA_BIT(DeviceAddress, 2, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 0, 1) ; 
  wait 100 ms
  OUT_GPB_BIT(DeviceAddress, 0, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 1, 1) ; 
  wait 100 ms
  OUT_GPB_BIT(DeviceAddress, 1, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 2, 1) ; 
  wait 100 ms
  OUT_GPB_BIT(DeviceAddress, 2, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 3, 1) ; 
  wait 100 ms
  OUT_GPB_BIT(DeviceAddress, 3, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 4, 1) ; 
  wait 100 ms
  OUT_GPB_BIT(DeviceAddress, 4, 0) ; 
  OUT_GPB_BIT(DeviceAddress, 5, 1) ; 
  wait 100 ms
Loop
