;Program compiled by GCBASIC (2025.01.10 (Linux 64 bit) : Build 1450) for Microchip MPASM/MPLAB-X Assembler using FreeBASIC 1.10.1/2025-07-19 CRC247
;Need help? 
;  Please donate to help support the operational costs of the project.  Donate via https://gcbasic.com/donate/
;  
;  See the GCBASIC forums at http://sourceforge.net/projects/gcbasic/forums,
;  Check the documentation and Help at http://gcbasic.sourceforge.net/help/,
;or, email us:
;   evanvennn at users dot sourceforge dot net
;********************************************************************************
;   Installation Dir : /opt/GCBASIC
;   Source file      : Blink.gcb
;   Setting file     : /opt/GCBASIC/use.ini
;   Preserve mode    : 2
;   Assembler        : GCASM
;   Programmer       : pp3
;   Output file      : Blink.asm
;   Float Capability : 1
;********************************************************************************

;Set up the assembler options (Chip type, clock source, other bits and pieces)
 LIST p=16F1455, r=DEC
 TITLE       "Blink.gcb"
 SUBTITLE    "05-10-2026 20:03:03"
#include <P16F1455.inc>
 __CONFIG _CONFIG1, _FCMEN_ON & _CLKOUTEN_OFF & _CP_OFF & _MCLRE_ON & _WDTE_OFF & _FOSC_INTOSC
 __CONFIG _CONFIG2, _LVP_ON & _PLLEN_DISABLED & _CPUDIV_NOCLKDIV & _WRT_OFF

;********************************************************************************

;Set aside memory locations for variables
;  Shared/Access RAM = (SA)
DELAYTEMP                        EQU     112          ; 0x70 (SA)
DELAYTEMP2                       EQU     113          ; 0x71 (SA)
SYSTEMP1                         EQU      32          ; 0x20
SYSWAITTEMPMS                    EQU     114          ; 0x72 (SA)
SYSWAITTEMPMS_H                  EQU     115          ; 0x73 (SA)

;********************************************************************************

;Vectors
	ORG	0
	pagesel	BASPROGRAMSTART
	goto	BASPROGRAMSTART
	ORG	4
	retfie

;********************************************************************************

;Program_memory_page: 0
	ORG	5
BASPROGRAMSTART
;Call initialisation routines
	call	INITSYS

;Start_of_the_main_program
;デフォルトで off になっててなんか怖い
;Dir PortA.5 Out
	banksel	TRISA
	bcf	TRISA,5
;Do
SysDoLoop_S1
;PortA.5 = !PortA.5
	banksel	SYSTEMP1
	clrf	SysTemp1
	btfsc	PORTA,5
	incf	SysTemp1,F
	comf	SysTemp1,F
	banksel	LATA
	bcf	LATA,5
	banksel	SYSTEMP1
	btfss	SysTemp1,0
	goto	ENDIF2
	banksel	LATA
	bsf	LATA,5
ENDIF2
;Wait 500 ms
	movlw	244
	movwf	SysWaitTempMS
	movlw	1
	movwf	SysWaitTempMS_H
	banksel	STATUS
	call	Delay_MS
;Loop
	goto	SysDoLoop_S1
SysDoLoop_E1
BASPROGRAMEND
	sleep
	goto	BASPROGRAMEND

;********************************************************************************

Delay_MS
	incf	SysWaitTempMS_H, F
DMS_START
	movlw	4
	movwf	DELAYTEMP2
DMS_OUTER
	movlw	165
	movwf	DELAYTEMP
DMS_INNER
	decfsz	DELAYTEMP, F
	goto	DMS_INNER
	decfsz	DELAYTEMP2, F
	goto	DMS_OUTER
	decfsz	SysWaitTempMS, F
	goto	DMS_START
	decfsz	SysWaitTempMS_H, F
	goto	DMS_START
	return

;********************************************************************************

;Source: system.h (198)
INITSYS
;OSCCON type is 105 'Bit(SPLLEN) Or Bit(IRCF3) And NoBit(INTSRC) and ifdef Bit(IRCF3)
;equates to OSCCON = OSCCON AND b'10000111' &  OSCCON = OSCCON OR b'01110000'
;= 8Mhz
;Set IRCF3 On
	banksel	OSCCON
	bsf	OSCCON,IRCF3
;Set IRCF2 On
	bsf	OSCCON,IRCF2
;Set IRCF1 On
	bsf	OSCCON,IRCF1
;Set IRCF0 Off
	bcf	OSCCON,IRCF0
;Set SPLLEN Off
	bcf	OSCCON,SPLLEN
;_Complete_the_chip_setup_of_BSR_ADCs_ANSEL_and_other_key_setup_registers_or_register_bits
;Ensure all ports are set for digital I/O and, turn off A/D
;SET ADFM OFF
	bcf	ADCON1,ADFM
;Switch off A/D Var(ADCON0)
;SET ADCON0.ADON OFF
	bcf	ADCON0,ADON
;ANSELA = 0
	banksel	ANSELA
	clrf	ANSELA
;ANSELC = 0
	clrf	ANSELC
;Set comparator register bits for many MCUs with register CM2CON0
;C2ON = 0
	banksel	CM2CON0
	bcf	CM2CON0,C2ON
;C1ON = 0
	bcf	CM1CON0,C1ON
;Turn off all ports
;PORTA = 0
	banksel	PORTA
	clrf	PORTA
;PORTC = 0
	clrf	PORTC
	return

;********************************************************************************

;Program_memory_page: 1
	ORG	2048
;Program_memory_page: 2
	ORG	4096
;Program_memory_page: 3
	ORG	6144

 END
