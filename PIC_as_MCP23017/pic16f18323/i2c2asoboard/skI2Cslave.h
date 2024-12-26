// skI2Cslave.h
// Ｉ２Ｃ通信のスレーブ関数用ライブラリ用インクルードファイル
// 送受信を行うデータのサイズによりSND_DATA_LEN/RCV_DATA_LENを調整して下さい。
#ifndef _SKI2CSLAVE_H_
#define _SKI2CSLAVE_H_

#define SND_DATA_LEN 8                  // 送信データバッファのサイズ
#define RCV_DATA_LEN 8                  // 受信データバッファのサイズ

unsigned char rcv_data[RCV_DATA_LEN] ;  // 受信データバッファ
unsigned char snd_data[SND_DATA_LEN] ;  // 送信データバッファ


void interrupt InterI2C( void ) ;
void InitI2C_Slave(int address) ;
int I2C_ReceiveCheck() ;

#endif
