/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021, 2025 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : r_cg_interrupt_handlers.h
* Version          : 1.0.50
* Device(s)        : R7F101G6ExSP
* Description      : None
***********************************************************************************************************************/

#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H
/*
 * INT_WDTI (0x4)
 */
void INT_WDTI (void) __attribute__ ((interrupt));

/*
 * INT_LVI (0x6)
 */
void INT_LVI (void) __attribute__ ((interrupt));

/*
 * INT_P0 (0x8)
 */
void INT_P0 (void) __attribute__ ((interrupt));

/*
 * INT_P1/INTAD1 (0xA)
 */
void INT_P1 (void) __attribute__ ((interrupt));
//void INT_AD1 (void) __attribute__ ((interrupt));

/*
 * INT_P2/INTAD2  (0xC)
 */
void INT_P2 (void) __attribute__ ((interrupt));
//void INTAD2 (void) __attribute__ ((interrupt));

/*
 * INT_P3 (0xE)
 */
void INT_P3 (void) __attribute__ ((interrupt));

/*
 * INT_P4 (0x10)
 */
void INT_P4 (void) __attribute__ ((interrupt));

/*
 * INT_P5 (0x12)
 */
void INT_P5 (void) __attribute__ ((interrupt));

/*
 * INT_CSI20/INT_IIC20/INT_ST2 (0x14)
 */
void INT_ST2 (void) __attribute__ ((interrupt));
//void INT_CSI20 (void) __attribute__ ((interrupt));
//void INT_IIC20 (void) __attribute__ ((interrupt));

/*
 * INT_CSI21/INT_IIC21/INT_SR2 (0x16)
 */
void INT_SR2 (void) __attribute__ ((interrupt));
//void INT_CSI21 (void) __attribute__ ((interrupt));
//void INT_IIC21 (void) __attribute__ ((interrupt));

/*
 * INT_SRE2 (0x18)
 */
void INT_SRE2 (void) __attribute__ ((interrupt));

/*
 * INT_FAAE (0x1A)
 */
void INT_FAAE (void) __attribute__ ((interrupt));

/*
 * INT_TIMEC0 (0x1C)
 */
void INT_TIMEC0 (void) __attribute__ ((interrupt));

/*
 * INT_CSI00/INT_IIC00/INT_ST0 (0x1E)
 */
void INT_ST0 (void) __attribute__ ((interrupt));
//void INT_CSI00 (void) __attribute__ ((interrupt));
//void INT_IIC00 (void) __attribute__ ((interrupt));

/*
 * INT_TM00 (0x20)
 */
void INT_TM00 (void) __attribute__ ((interrupt));

/*
 * INT_SRE0/INT_TM01H (0x22)
 */
void INT_TM01H (void) __attribute__ ((interrupt));
//void INT_SRE0 (void) __attribute__ ((interrupt));

/*
 * INT_CSI10/INT_IIC10/INT_ST1 (0x24)
 */
void r_Config_UART1_interrupt_send (void) __attribute__ ((interrupt));

/*
 * INT_CSI11/INT_IIC11/INT_SR1 (0x26)
 */
void INT_SR1 (void) __attribute__ ((interrupt));
//void INT_CSI11 (void) __attribute__ ((interrupt));
//void INT_IIC11 (void) __attribute__ ((interrupt));

/*
 * INT_SRE1/INT_TM03H (0x28)
 */
void INT_TM03H (void) __attribute__ ((interrupt));
//void INT_SRE1 (void) __attribute__ ((interrupt));

/*
 * INT_IICA0 (0x2A)
 */
void r_Config_IICA0_interrupt (void) __attribute__ ((interrupt));

/*
 * INT_CSI01/INT_IIC01/INT_SR0 (0x2C)
 */
void INT_SR0 (void) __attribute__ ((interrupt));
//void INT_CSI01 (void) __attribute__ ((interrupt));
//void INT_IIC01 (void) __attribute__ ((interrupt));

/*
 * INT_TM01 (0x2E)
 */
void INT_TM01 (void) __attribute__ ((interrupt));

/*
 * INT_TM02 (0x30)
 */
void INT_TM02 (void) __attribute__ ((interrupt));

/*
 * INT_TM03 (0x32)
 */
void INT_TM03 (void) __attribute__ ((interrupt));

/*
 * INT_AD0 (0x34)
 */
void r_Config_ADC_interrupt (void) __attribute__ ((interrupt));

/*
 * INT_RTC (0x36)
 */
void INT_RTC (void) __attribute__ ((interrupt));

/*
 * INT_ITL (0x38)
 */
void INT_ITL (void) __attribute__ ((interrupt));

/*
 * INT_KR (0x3A)
 */
void INT_KR (void) __attribute__ ((interrupt));

/*
 * INT_TD/INT_TRJ0 (0x3C)
 */
void INT_TRJ0 (void) __attribute__ ((interrupt));
//void INT_TD (void) __attribute__ ((interrupt));

/*
 * INT_TRD0 (0x3E)
 */
void INT_TRD0 (void) __attribute__ ((interrupt));

/*
 * INT_TRD1 (0x40)
 */
void INT_TRD1 (void) __attribute__ ((interrupt));

/*
 * INT_RD/INT_TRG (0x42)
 */
void INT_TRG (void) __attribute__ ((interrupt));
//void INT_RD (void) __attribute__ ((interrupt));

/*
 * INT_ED/INT_CLD/INT_BPD/INT_TRX (0x44)
 */
void INT_TRX (void) __attribute__ ((interrupt));
//void INT_ED (void) __attribute__ ((interrupt));
//void INT_CLD (void) __attribute__ ((interrupt));
//void INT_BPD (void) __attribute__ ((interrupt));

/*
 * INT_P20 (0x46)
 */
void INT_P20 (void) __attribute__ ((interrupt));

/*
 * INT_P21 (0x48)
 */
void INT_P21 (void) __attribute__ ((interrupt));

/*
 * INT_P6 (0x4A)
 */
void INT_P6 (void) __attribute__ ((interrupt));

/*
 * INT_P7/INT_TMKBSTP10 (0x4C)
 */
void INT_P7 (void) __attribute__ ((interrupt));
//void INT_TMKBSTP10 (void) __attribute__ ((interrupt));

/*
 * INT_P8/INT_TMKBSTR11 (0x4E)
 */
void INT_P8 (void) __attribute__ ((interrupt));
//void INT_TMKBSTR11 (void) __attribute__ ((interrupt));

/*
 * INT_P9/INT_TMKBSTP11 (0x50)
 */
void INT_P9 (void) __attribute__ ((interrupt));
//void INT_TMKBSTP11 (void) __attribute__ ((interrupt));

/*
 * INT_FL (0x52)
 */
void INT_FL (void) __attribute__ ((interrupt));

/*
 * INT_CMP0/INT_P10 (0x54)
 */
void INT_P10 (void) __attribute__ ((interrupt));
//void INT_CMP0 (void) __attribute__ ((interrupt));

/*
 * INT_CMP1/INT_P11 (0x56)
 */
void INT_P11 (void) __attribute__ ((interrupt));
//void INT_CMP1 (void) __attribute__ ((interrupt));

/*
 * INT_FAATRAP (0x58)
 */
void INT_FAATRAP (void) __attribute__ ((interrupt));

/*
 * INT_CMP2 (0x5A)
 */
void INT_CMP2 (void) __attribute__ ((interrupt));

/*
 * INT_CMP3 (0x5C)
 */
void INT_CMP3 (void) __attribute__ ((interrupt));

/*
 * INT_TMKB0 (0x5E)
 */
void INT_TMKB0 (void) __attribute__ ((interrupt));

/*
 * INT_TMKB1 (0x60)
 */
void INT_TMKB1 (void) __attribute__ ((interrupt));

/*
 * INT_TMKB2 (0x62)
 */
void INT_TMKB2 (void) __attribute__ ((interrupt));

/*
 * INT_SDD/INT_GCR (0x64)
 */
void INT_GCR (void) __attribute__ ((interrupt));
//void INT_SDD (void) __attribute__ ((interrupt));

/*
 * INT_FED/INT_PMC (0x66)
 */
void INT_PMC (void) __attribute__ ((interrupt));
//void INT_FED (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTR00 (0x68)
 */
void INT_TMKBSTR00 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTP00 (0x6A)
 */
void INT_TMKBSTP00 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTR01 (0x6C)
 */
void INT_TMKBSTR01 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTP01 (0x6E)
 */
void INT_TMKBSTP01 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTR20 (0x70)
 */
void INT_TMKBSTR20 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTP20 (0x72)
 */
void INT_TMKBSTP20 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTR21 (0x74)
 */
void INT_TMKBSTR21 (void) __attribute__ ((interrupt));

/*
 * INT_TMKBSTP21 (0x76)
 */
void INT_TMKBSTP21 (void) __attribute__ ((interrupt));

/*
 * INT_TIMEC1 (0x78)
 */
void INT_TIMEC1 (void) __attribute__ ((interrupt));

/*
 * INT_TIMEC2 (0x7A)
 */
void INT_TIMEC2 (void) __attribute__ ((interrupt));

/*
 * INT_AD3 (0x7C)
 */
void INT_AD3 (void) __attribute__ ((interrupt));

/*
 * INT_BRK_I (0x7E)
 */
void INT_BRK_I (void) __attribute__ ((interrupt));

//Hardware Vectors
//PowerON_Reset (0x0)
void PowerON_Reset (void) __attribute__ ((interrupt));

#endif