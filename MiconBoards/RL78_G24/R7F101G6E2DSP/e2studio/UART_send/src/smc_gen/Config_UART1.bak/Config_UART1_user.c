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
* File Name        : Config_UART1_user.c
* Component Version: 1.10.0
* Device(s)        : R7F101G6ExSP
* Description      : This file implements device driver for Config_UART1.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_UART1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart1_tx_address;    /* uart1 transmit buffer address */
extern volatile uint16_t g_uart1_tx_count;        /* uart1 transmit data number */
extern volatile uint8_t _uart_end;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART1_Create_UserInit
* Description  : This function adds user code after initializing UART1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART1_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_UART1_callback_sendend
* Description  : This function is a callback function when UART1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_Config_UART1_callback_sendend(void)
{
    /* Start user code for r_Config_UART1_callback_sendend. Do not edit comment generated here */
	_uart_end=1;
	return;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_UART1_interrupt_send
* Description  : This function is UART1 send interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_Config_UART1_interrupt_send(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
    else
    {
        r_Config_UART1_callback_sendend();
    }
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: OreUART1_Send_Txt
* Description  : R_Config_UART1_Send で文字を送って、完了を待つ
* Arguments    : (unsigned char *)txt 
*              : - txt は char txt[]="hoge\r\n" のように文字列を指定
*              : num - 文字数。"hoge\r\n" なら num=6
* Return Value : None
***********************************************************************************************************************/
void OreUART1_Send_Txt( uint8_t * const txt, uint16_t num)
{
/* {{{ */
  _uart_end=0;
  R_Config_UART1_Send( (unsigned char *)txt, num);
  while(_uart_end == 0){};
  _uart_end=0;
/* }}} */
}

/***********************************************************************************************************************
* Function Name: OreUART1_Send_U16
* Description  : 16ビットの正の整数を 10進数で表示
* Arguments    : num - 16ビットの正の整数
* Return Value : None
***********************************************************************************************************************/
void OreUART1_Send_U16( uint16_t num)
{
/* {{{ */
  uint8_t b; // その桁の値
  uint8_t flag=0; // 最初の桁を見つけるフラグ

  // 10000 で割る。答が 0 ならスキップ
  flag=flag | (uint8_t)(num/10000); 
  if(flag != 0){
    b=(uint8_t)(num/10000)+48;
    OreUART1_Send_ASCII(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%10000;
  };

  flag=flag | (uint8_t)(num/1000); 
  if(flag != 0){
    b=(uint8_t)(num/1000)+48;
    OreUART1_Send_ASCII(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%1000;
  };

  flag=flag | (uint8_t)(num/100); 
  if(flag != 0){
    b=(uint8_t)(num/100)+48;
    OreUART1_Send_ASCII(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%100;
  };

  flag=flag | (uint8_t)(num/10); 
  if(flag != 0){
    b=(uint8_t)(num/10)+48;
    OreUART1_Send_ASCII(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%10;
  };

  // 最後は 0 でもなんでも表示するのでそのまま送る。
  num=num%10+48;
  OreUART1_Send_ASCII((uint8_t)num);
//  OreUART1_Send_Txt( (unsigned char *)&num, 1);
/* }}} */
}

/***********************************************************************************************************************
* Function Name: OreUART1_Send_Float
* Description  : Float 型を指数形式で表示
* Arguments    : val - float の実数
*              : num - 数字部分の小数点以下の文字数 1.23e-3 なら2 
* Return Value : None
***********************************************************************************************************************/
void OreUART1_Send_Float( float val, uint8_t num)
{
/* {{{ */
  uint8_t sign=0; //符号:0 正, 1 負
  char keta=0; // e-8 みたいな桁数
  uint8_t keta_sign=0; // keta の符号: 1で負
  uint8_t i; //汎用

  if(val < 0.0){
    sign=1;
    val=val*-1.0; //val から符号を取る。符号は sign が記憶
  };


  // float は e-38 から e+38 まで
  if(val==1.0){
    keta=0;
  } else if(val>1.0){
    while(val > 1.0){
      val=val/10.0;
      keta=keta+1;
      //12.3 と来たとき 
      //1回目 val=1.23, keta=1
      //2回目 val=0.123, keta=2
      //1桁下げすぎたな
    };
    val=val*10.0; //0.123 に下がったのが 1.23 になる
    keta=keta-1; // keta 2 --> 1 
  } else {
    while(val < 1.0){
      val=val*10.0;
      keta=keta-1;
      // 0.0123 と来た場合
      // 1回目 val=0.123, keta=-1
      // 2回目 val=1.23, keta=-2
      // 丁度良い
    };
  };

  if(keta < 0){
    keta_sign=1;
    keta=keta*-1;
  };
  // 準備は整ったはず

  // 負なら "-" を出す
  if(sign==1){
    OreUART1_Send_ASCII(45 );
  };
  
  //最初の桁を出力
  // 1.23 みたいなら 1 って出るはず。
  OreUART1_Send_U16( (uint8_t)val );
  OreUART1_Send_ASCII(46 ); // dot

  val=(float)(val- (uint8_t)val); //val=1.23 だったなら val=0.23 のはず

  // 残りの桁を出そう
  for(i=0;i<num;i++){
    val=val * 10.0; //0.23 なら 2.3
    OreUART1_Send_U16( (uint8_t)val ); //2 が出るはず
    val=(float)(val- (uint8_t)val); //2.3 → 0.3 になるはず
  };

  // "e" を出力
  OreUART1_Send_ASCII(101 ); // e

  // 負なら "-" を出す
  if(keta_sign==1){
    OreUART1_Send_ASCII(45 );
//    OreUART1_Send_Txt( (unsigned char *)str_minus,1);
  };

  // 指数値を出す
  OreUART1_Send_U16( (uint8_t) keta);

  return;
/* }}} */
}

/***********************************************************************************************************************
* Function Name: OreUART1_Send_CRLF
* Description  : "\r\n" を送る
* Arguments    : 無し
* Return Value : None
***********************************************************************************************************************/
void OreUART1_Send_CRLF( )
{
  OreUART1_Send_ASCII(13 ); //"\r"
  OreUART1_Send_ASCII(10 ); //"\n"
}

/***********************************************************************************************************************
* Function Name: OreUART1_Send_ASCII
* Description  : AScii code をそのまま送る
* Arguments    : uint8_t code - ascii code
* Return Value : None
***********************************************************************************************************************/
void OreUART1_Send_ASCII(uint8_t code )
{
  OreUART1_Send_Txt( (unsigned char *)&code,1);
}

/* End user code. Do not edit comment generated here */
