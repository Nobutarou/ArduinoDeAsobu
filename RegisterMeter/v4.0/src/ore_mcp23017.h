; The original is /opt/GCBASIC/include/mcp23017.h
#define DeviceAddress 0x40
'       ================================================================================
'       '                   Register Definitions (Bank = 0)
'       ================================================================================
'       I/O DIRECTION REGISTER
'       Controls the direction of the data I/O.
'       When a bit is set, the corresponding pin becomes an input.
'       When a bit is clear, the corresponding pin becomes an output.
        #Define IODIRA 0x00 ' register GPIOA
        #Define IODIRB 0x01 ' register GPIOB

'       INPUT POLARITY REGISTER
'       This register allows the user to configure the polarity on
'       the corresponding GPIO port bits.
'       If a bit is set, the corresponding GPIO register bit will
'       reflect the inverted value on the pin.
        #Define IOPOLA 0x02        ' PORTA A
        #Define IOPOLB 0x03        ' PORTA B

'       INTERRUPT-ON-CHANGE CONTROL REGISTER
'       The GPINTEN register controls the interrupt-onchange
'       feature for each pin.
'       If a bit is set, the corresponding pin is enabled for
'       interrupt-on-change. The DEFVAL and INTCON
'       registers must also be configured if any pins are
'       enabled for interrupt-on-change.
        #Define GPINTENA 0x04
        #Define GPINTENB 0x05

'       DEFAULT COMPARE REGISTERF OR INTERRUPT-ON-CHANGE
'       The default comparison value is configured in the DEFVAL register.
'       If enabled (via GPINTEN and INTCON) to compare against the DEFVAL register,
'       an opposite value on the associated pin will cause aninterrupt to occur.
        #Define DEFVALA 0x06
        #Define DEFVALB 0x07

'       INTERRUPT CONTROL REGISTER
'       The INTCON register controls how the associated pin value is compared
'       for the interrupt-on-change feature.
'       If a bit is set, the corresponding I/O pin is compareda gainst the
'       associated bit in the DEFVAL register.
'       If abt value is clear, the corresponding I/O pin is compared
'       against the previous value.
        #Define INTCONA = 0x08
        #Define INTCONB = 0x09

'       IOCON � I/O EXPANDER CONFIGURATION REGISTER See datasheet
        #Define IOCONA 0x0A
        #Define IOCONB 0x0B

'       PULL-UP RESISOR CONFIGURATION REGISTER
'       The GPPU register controls the pull-up resistors for the port pins.
'       If a bit is set and the corresponding pin is configured as an input,
'       the corresponding port pin is internally pulled up with a 100k resistor.
        #Define GPPUA 0x0C
        #Define GPPUB 0x0D

'       INTERRUPT FLAG REGISTER
'       The INTF register reflects the interrupt condition on the port pins of
'       any pin that is enabled for interrupts via the GPINTEN register.
'       A �set� bit indicates that the associated pin caused the interrupt.
'       This register is �read-only�.
'       Writes to this register will be ignored.
        #Define INTFA 0x0E
        #Define INTFB 0x0F

'       INTERRUPT CAPTURE REGISTER
'       The INTCAP register captures the GPIO port value at
'       the time the interrupt occurred.
'       The register is �reado nly� and is updated only when an interrupt occurs.
'       The register will remain unchanged until the interrupt is
'       cleared via a read of INTCAP or GPIO.
        #Define INTCAPA 0x10
        #Define INTCAPB 0x11

'       PORT REGISTER
'       The GPIO register reflects the value on the port.
'       Reading from this register reads the port.
'       Writing to this register modifies the Output Latch (OLAT) register.
        #Define GPIOA 0x12 ' address port GPIOA
        #Define GPIOB 0x13 ' address port GPIOB

'       OUTPUT LATCH REGISTER (OLAT)
'       The OLAT register provides access to the output latches.
'       A read from this register results in a read of the
'       OLAT and not the port itself.
'       A write to this register modifies the output latches that
'       modifies the pins configured as outputs.
        #Define OLATA 0x14
        #Define OLATB 0x15
'       --- End of defines -----------------------------------------------------------



'       ==============================================================================
'       ==============================================================================
'       '             Procedures to send and receive data from MCP23017
'       ==============================================================================
'       ==============================================================================


'       ==============================================================================
'       Send a byte to MCP23017
'       ------------------------------------------------------------------------------
Sub MCP23017_SendByte (In MCP23017_device, In MCP23017_instr , In MCP23017_data)
'       see the Data sheet for this!!! FIGURE 1-1: MCP23017 I2C DEVICE PROTOCOL
'       -
        #IfDef I2C_DATA
           I2CStart
           I2CSend( MCP23017_device )
           I2CSend( MCP23017_instr )
           I2CSend( MCP23017_data )
           I2CStop
        #EndIf
'       -
        #IfDef HI2C_DATA
           HI2CRestart                           ;generate a start signal
           HI2CSend( MCP23017_device )
           HI2CSend( MCP23017_instr )
           HI2CSend( MCP23017_data )
           HI2CStop
        #EndIf
End Sub


'       ==============================================================================
'       Receive a byte from MCP23017
'       ------------------------------------------------------------------------------
Sub MCP23017_ReadByte (In MCP23017_device, In MCP23017_instr , Out MCP23017_data)
'       see the Data sheet for this!!! FIGURE 1-1: MCP23017 I2C DEVICE PROTOCOL
'       -
        #IfDef I2C_DATA
           I2CStart
           I2CSend( MCP23017_device )
           I2CSend( MCP23017_instr )
           I2CStart
           I2CSend( MCP23017_device + 1)
           I2CReceive( MCP23017_data , NACK )
           I2CStop
        #EndIf
'       -
        #IfDef HI2C_DATA
           HI2Crestart
           HI2CSend( MCP23017_device )
           HI2CSend( MCP23017_instr )
           HI2CRestart
           HI2CSend( MCP23017_device + 1)
           HI2CReceive( MCP23017_data , NACK )
           HI2CStop
        #EndIf
End Sub


'       ==============================================================================
'       Set port A to IO
'       ------------------------------------------------------------------------------
Sub INIT_MCP23017_GPA (In DeviceAddress, In State_A)
        MCP23017_SendByte (DeviceAddress, IODIRA, State_A)
        Wait 1 MS
End Sub


'       ==============================================================================
'       Set port B to IO
'       ------------------------------------------------------------------------------
Sub INIT_MCP23017_GPB (In DeviceAddress, In State_B)
        MCP23017_SendByte (DeviceAddress, IODIRB, State_b)
        Wait 1 ms
End Sub


'       ==============================================================================
'       DEFAULT COMPARE REGISTERF OR INTERRUPT-ON-CHANGE
'       The default comparison value is configured in the DEFVAL register.
'       If enabled (via GPINTEN and INTCON) to compare against the DEFVAL register,
'       an opposite value on the associated pin will cause an interrupt to occur.
'       #define DEFVALA = 0x06
'       #define DEFVALB = 0x07
'       -------------------------------------------------------------------------------
Sub SetDefValA (In DeviceAddress, In Valore)
        MCP23017_SendByte (DeviceAddress, DEFVALA, Valore)
End Sub

Sub SetDefValB (In DeviceAddress, In Valore)
        MCP23017_SendByte (DeviceAddress, DEFVALB, Valore)
End Sub


'       ==============================================================================
'       INPUT POLARITY REGISTER
'       This register allows the user to configure the polarity on
'       the corresponding GPIO port bits.
'       If a bit is set, the corresponding GPIO register bit will
'       reflect the inverted value on the pin. (Solo la lettura)
'       #Define IOPOLA = 0x02        ' PORTA A
'       #Define IOPOLB = 0x03        ' PORTA B
'       ------------------------------------------------------------------------------
Sub SetIOPolA (In DeviceAddress, In Pol)
        MCP23017_SendByte (DeviceAddress, IOPOLA, Pol)
End Sub

Sub SetIOPolB (In DeviceAddress, In Pol)
        MCP23017_SendByte (DeviceAddress, IOPOLB, Pol)
End Sub


'       ==============================================================================
'       Configurazione Pull-Up della PORTA A
'       Se il bit � settato a 1 il pin corrispondente, se configurato
'       come input, � pullappato alto internamente con una resistenza da 100K.
'       Pull-Up configuration of PORT A
'       If the bit is set to 1, the corresponding pin, if configured
'       as input, is internally high-lapped with a 100K resistor.
'       ------------------------------------------------------------------------------
Sub ConfigurePullUps_GPA (In DeviceAddress, In PullA)
        MCP23017_SendByte (DeviceAddress, GPPUA, PullA)
End Sub


'       ==============================================================================
'       Configurazione Pull-Up della PORTA B
'       Se il bit � settato a 1 il pin corrispondente, se configurato
'       come input, � pullappato alto internamente con una resistenza da 100K.
'       Pull-Up configuration of PORT B
'       If the bit is set to 1, the corresponding pin, if configured
'       as input, is internally high-lapped with a 100K resistor.
'       ------------------------------------------------------------------------------
Sub ConfigurePullUps_GPB (In DeviceAddress, In PullB)
        MCP23017_SendByte (DeviceAddress, GPPUB, PullB)
End Sub


'       ==============================================================================
'       INTERRUPT FLAG REGISTER   (READ)
'       The INTF register reflects the interrupt condition on the port pins of
'       any pin that is enabled for interrupts via the GPINTEN register.
'       A �set� bit indicates that the associated pin caused the interrupt.
'       This register is �read-only�.
'       Writes to this register will be ignored.
'       -------------------------------------------------------------------------------
Function ReadInterruptFlagA (In DeviceAddress) As Byte
        MCP23017_ReadByte (DeviceAddress, INTFA, ReadInterruptFlagA)
End Function

Function ReadInterruptFlagB (In DeviceAddress) As Byte
        MCP23017_ReadByte (DeviceAddress, INTFB, ReadInterruptFlagB)
End Function


'       ==============================================================================
'       INTERRUPT CAPTURE REGISTER (READ)
'       The INTCAP register captures the GPIO port value at
'       the time the interrupt occurred.
'       The register is �reado nly� and is updated only when an interrupt occurs.
'       The register will remain unchanged until the interrupt is
'       cleared via a read of INTCAP or GPIO.
'       ------------------------------------------------------------------------------
Function ReadIntCapA (In DeviceAddress As Byte) As Byte
        MCP23017_ReadByte (DeviceAddress, INTCAPA, ReadInCapA)
End Function

Function ReadIntCapB (In DeviceAddress As Byte) As Byte
        MCP23017_ReadByte (DeviceAddress, INTCAPB, ReadInCapB)
End Function


'       ==============================================================================
'       INTERRUPT-ON-CHANGE CONTROL REGISTER
'       The GPINTEN register controls the interrupt-onchange
'       feature for each pin.
'       If a bit is set, the corresponding pin is enabled for
'       interrupt-on-change. The DEFVAL and INTCON
'       registers must also be configured if any pins are
'       enabled for interrupt-on-change.
'       #define GPINTENA = 0x04
'       #define GPINTENB = 0x05
'       -------------------------------------------------------------------------------
Sub SetGpIntA (In DeviceAddress, In IntA)
        MCP23017_SendByte (DeviceAddress, GPINTENA, IntA)
End Sub

Sub SetGpIntB (In DeviceAddress, In IntB)
        MCP23017_SendByte (DeviceAddress, GPINTENB, IntB)
End Sub


'       ==============================================================================
'       INTERRUPT CONTROL REGISTER
'       The INTCON register controls how the associated pin value is compared
'       for the interrupt-on-change feature.
'       If a bit is set, the corresponding I/O pin is compareda against the
'       associated bit in the DEFVAL register.
'       If abt value is clear, the corresponding I/O pin is compared
'       against the previous value.
'       -------------------------------------------------------------------------------
Sub SetIntConA (In DeviceAddress, In IntC)
        MCP23017_SendByte (DeviceAddress, INTCONA, IntC)
End Sub

Sub SetIntConB (In DeviceAddress, In IntC)
        MCP23017_SendByte (DeviceAddress, INTCONB, IntC)
End Sub


'       ==============================================================================
'       Programma tutte le uscite della porta A inviando un byte
'       Program all outputs of port A by sending a byte
'       ------------------------------------------------------------------------------
Sub OUT_GPA_BYTE(In DeviceAddress, In Data_A)
        MCP23017_SendByte (DeviceAddress, GPIOA, Data_A)
End Sub


'       ==============================================================================
'       Programma tutte le uscite della porta B invianto un byte
'       Program all outputs of port B by sending a byte
'       ------------------------------------------------------------------------------
Sub OUT_GPB_BYTE(In DeviceAddress, In Data_B)
        MCP23017_SendByte (DeviceAddress, GPIOB, Data_B)
End Sub


'       ==============================================================================
'       Legge tutte le uscite della porta A leggendo un byte
'       Read all the outputs of port A by reading a byte
'       ------------------------------------------------------------------------------
Function IN_GPA_BYTE (In DeviceAddress As Byte) As Byte
        MCP23017_ReadByte (DeviceAddress, GPIOA, IN_GPA_BYTE)
End Function


'       ==============================================================================
'       Legge tutte le uscite della porta B leggendo un byte
'       Read all the outputs of port B by reading a byte
'       ------------------------------------------------------------------------------
Function IN_GPB_BYTE (In DeviceAddress As Byte) As Byte
        MCP23017_ReadByte (DeviceAddress, GPIOB, IN_GPB_BYTE)
End Function


        Dim DataGpAByte As Byte
        Dim DataGpBByte As Byte
'       ==============================================================================
'       Programma un solo bit della porta A
'       Program only one bit of port A
'       ------------------------------------------------------------------------------
Sub OUT_GPA_BIT (In DeviceAddress, In BIT_A, In State_Bit)
        DataGpAByte = IN_GPA_BYTE (DeviceAddress)
        If State_Bit Then
           DataGpAByte.BIT_A = 1
        Else
           DataGpAByte.BIT_A = 0
        End If
        OUT_GPA_BYTE (DeviceAddress, DataGpAByte)
End Sub

'       ==============================================================================
'       Programma un solo bit della porta B
'       Program only one bit of port B
'       ------------------------------------------------------------------------------
Sub OUT_GPB_BIT (In DeviceAddress, In Bit_B, In State_Bit)
        DataGpBByte = IN_GPB_BYTE (DeviceAddress)
        If State_Bit Then
           DataGpBByte.BIT_B = 1
        Else
           DataGpBByte.BIT_B = 0
        End If
        OUT_GPB_BYTE(DeviceAddress, DataGpBByte)
End Sub


'       ==============================================================================
'       Legge un solo bit dalla porta A
'       Read only one bit from port A
'       ------------------------------------------------------------------------------
Function IN_GPA_BIT (In DeviceAddress, In BIT_A) As Byte
        DataGpAByte = IN_GPA_BYTE (DeviceAddress)
        IN_GPA_BIT = DataGpAByte.BIT_A
End Function


'       ==============================================================================
'       Legge un solo bit dalla porta B
'       Read only one bit from port B
'       ------------------------------------------------------------------------------
Function IN_GPB_BIT (In DeviceAddress, In BIT_B) As Byte
        DataGpBByte = IN_GPB_BYTE (DeviceAddress)
        IN_GPB_BIT = DataGpBByte.BIT_B
End Function

