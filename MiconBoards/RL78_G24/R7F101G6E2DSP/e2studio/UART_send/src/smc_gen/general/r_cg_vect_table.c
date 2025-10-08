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
* File Name        : r_cg_vect_table.c
* Version          : 1.0.50
* Device(s)        : R7F101G6ExSP
* Description      : None
***********************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

extern void PowerON_Reset (void);

const unsigned char Option_Bytes[]  __attribute__ ((section (".option_bytes"))) = {
    0xEFU, 0x3AU, 0xE9U, 0x04U
};

const unsigned char Security_Id[]  __attribute__ ((section (".security_id"))) = {
    0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U,0x00U
};

#define VEC          __attribute__ ((section (".vec")))
const void __near *HardwareVectors[] VEC = {
    // Address 0x0
    PowerON_Reset,
    // Secure for Debugging
    (void*)0xFFFF
};

#define VECT_SECT          __attribute__ ((section (".vects")))
const void __near *Vectors[] VECT_SECT = {
/*
 * INT_WDTI (0x4)
 */
INT_WDTI,

/*
 * INT_LVI (0x6)
 */
    INT_LVI,

/*
 * INT_P0 (0x8)
 */
    INT_P0,

/*
 * INT_P1/INT_AD1 (0xA)
 */
    INT_P1,
//    INT_AD1,

/*
 * INT_P2/INT_AD2 (0xC)
 */
    INT_P2,
//    INT_AD2,

/*
 * INT_P3 (0xE)
 */
    INT_P3,

/*
 * INT_P4 (0x10)
 */
    INT_P4,

/*
 * INT_P5 (0x12)
 */
    INT_P5,

/*
 * INT_CSI20/INT_IIC20/INT_ST2 (0x14)
 */
    INT_ST2,
//    INT_CSI20,
//    INT_IIC20,

/*
 * INT_CSI21/INT_IIC21/INT_SR2 (0x16)
 */
    INT_SR2,
//    INT_CSI21,
//    INT_IIC21,

/*
 * INT_SRE2 (0x18)
 */
    INT_SRE2,

/*
 * INT_FAAE (0x1A)
 */
    INT_FAAE,

/*
 * INT_TIMEC0 (0x1C)
 */
    INT_TIMEC0,

/*
 * INT_CSI00/INT_IIC00/INT_ST0 (0x1E)
 */
    INT_ST0,
//    INT_CSI00,
//    INT_IIC00,

/*
 * INT_TM00 (0x20)
 */
    INT_TM00,

/*
 * INT_SRE0/INT_TM01H (0x22)
 */
    INT_TM01H,
//    INT_SRE0,

/*
 * INT_CSI10/INT_IIC10/INT_ST1 (0x24)
 */
    r_Config_UART1_interrupt_send,

/*
 * INT_CSI11/INT_IIC11/INT_SR1 (0x26)
 */
    INT_SR1,
//    INT_CSI11,
//    INT_IIC11,

/*
 * INT_SRE1/INT_TM03H (0x28)
 */
    INT_TM03H,
//    INT_SRE1,

/*
 * INT_IICA0 (0x2A)
 */
    INT_IICA0,

/*
 * INT_CSI01/INT_IIC01/INT_SR0 (0x2C)
 */
    INT_SR0,
//    INT_CSI01,
//    INT_IIC01,

/*
 * INT_TM01 (0x2E)
 */
    INT_TM01,

/*
 * INT_TM02 (0x30)
 */
    INT_TM02,

/*
 * INT_TM03 (0x32)
 */
    INT_TM03,

/*
 * INT_AD0 (0x34)
 */
    INT_AD0,

/*
 * INT_RTC (0x36)
 */
    INT_RTC,

/*
 * INT_ITL (0x38)
 */
    INT_ITL,

/*
 * INT_KR (0x3A)
 */
    INT_KR,

/*
 * INT_TD/INT_TRJ0 (0x3C)
 */
    INT_TRJ0,
//    INT_TD,

/*
 * INT_TRD0 (0x3E)
 */
    INT_TRD0,

/*
 * INT_TRD1 (0x40)
 */
    INT_TRD1,

/*
 * INT_RD/INT_TRG (0x42)
 */
    INT_TRG,
//    INT_RD,

/*
 * INT_ED/INT_CLD/INT_BPD/INT_TRX (0x44)
 */
    INT_TRX,
//    INT_ED,
//    INT_CLD,
//    INT_BPD,

/*
 * INT_P20 (0x46)
 */
    INT_P20,

/*
 * INT_P21 (0x48)
 */
    INT_P21,

/*
 * INT_P6/INT_TMKBSTR10 (0x4A)
 */
    INT_P6,
//    INT_TMKBSTR10,

/*
 * INT_P7/INT_TMKBSTP10 (0x4C)
 */
    INT_P7,
//    INT_TMKBSTP10,

/*
 * INT_P8/INT_TMKBSTR11 (0x4E)
 */
    INT_P8,
//  INT_TMKBSTR11,

/*
 * INT_P9/INT_TMKBSTP11 (0x50)
 */
    INT_P9,
//  INT_TMKBSTP11,

/*
 * INT_FL (0x52)
 */
    INT_FL,

/*
 * INT_CMP0/INT_P10 (0x54)
 */
    INT_P10,
//    INT_CMP0,

/*
 * INT_CMP1/INT_P11 (0x56)
 */
    INT_P11,
//    INT_CMP1,

/*
 * INT_FAATRAP (0x58)
 */
    INT_FAATRAP,

/*
 * INT_CMP2 (0x5A)
 */
    INT_CMP2,

/*
 * INT_CMP3 (0x5C)
 */
    INT_CMP3,

/*
 * INT_TMKB0 (0x5E)
 */
    INT_TMKB0,

/*
 * INT_TMKB1 (0x60)
 */
    INT_TMKB1,

/*
 * INT_TMKB2 (0x62)
 */
    INT_TMKB2,

/*
 * INT_SDD/INT_GCR (0x64)
 */
    INT_GCR,
//    INT_SDD,

/*
 * INT_FED/INT_PMC (0x66)
 */
    INT_PMC,
//    INT_FED,

/*
 * INT_TMKBSTR00 (0x68)
 */
    INT_TMKBSTR00,

/*
 * INT_TMKBSTP00 (0x6A)
 */
    INT_TMKBSTP00,

/*
 * INT_TMKBSTR01 (0x6C)
 */
    INT_TMKBSTR01,

/*
 * INT_TMKBSTP01 (0x6E)
 */
    INT_TMKBSTP01,

/*
 * INT_TMKBSTR20 (0x70)
 */
    INT_TMKBSTR20,

/*
 * INT_TMKBSTP20 (0x72)
 */
    INT_TMKBSTP20,

/*
 * INT_TMKBSTR21 (0x74)
 */
    INT_TMKBSTR21,

/*
 * INT_TMKBSTP21 (0x76)
 */
    INT_TMKBSTP21,

/*
 * INT_TIMEC1 (0x78)
 */
    INT_TIMEC1,

/*
 * INT_TIMEC2 (0x7A)
 */
    INT_TIMEC2,

/*
 * INT_AD3 (0x7C)
 */
    INT_AD3,

/*
 * INT_BRK_I (0x7E)
 */
    INT_BRK_I,
};