// I2C MCP23008/17エミュレータ
// Copyrights(C) 2017 Tamotsu Kamigaki
//
// BANK0、シーケンシャルモード、割り込み不使用、正極性固定
//
// PIC16F1503 RC0/1:I2C SCL/SDA, RA0-RA5:A port 6bit I/O(RA3:inのみ) RC2-5:4bit PUなし

// Original: https://kikyoya.wordpress.com/2017/12/04/pic%E3%81%A7i-o%E3%82%A8%E3%82%AD%E3%82%B9%E3%83%91%E3%83%B3%E3%83%80%E3%83%BCmcp23008-mcp23017%E3%82%A8%E3%83%9F%E3%83%A5%E3%83%AC%E3%83%BC%E3%83%88/

// ビルドを成功させるには XC8 Global Options -> C standard -> C 90 にすべき

// エミュレートするチップを選択
#define EMU_MCP23017	// 17はBANK=0モードのAポート、BANK=1の時のAポートはEMU_MCP23008と同じ

#pragma jis				// 日本語文字列を使うための宣言
typedef unsigned char byte;

//////////////////////////////////////////////////////////////////////////////
// PIC16F18323
// CONFIG1
#pragma config FEXTOSC = OFF    // 内蔵水晶利用
#pragma config RSTOSC = HFINT32 // 32MHz 利用
#pragma config CLKOUTEN = OFF   // RA4 を I/O として使用
#pragma config CSWEN = ON       // NDIV でクロック落とせる
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor 使用しない

// CONFIG2
#pragma config MCLRE = OFF      // RA3 を input として利用
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT enabled, SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = OFF    // PPS は随時有効
#pragma config STVREN = ON      // Stack overflow でリセット
#pragma config DEBUG = OFF      // Debugger 不要 (使いこなせない)

// CONFIG3
#pragma config WRT = OFF        // ROM エリア書き込み OK
#pragma config LVP = ON         // これ禁止すると a-p-prog で書き込めないのでは？

// CONFIG4
#pragma config CP = OFF         // ROM エリア code protection なし。
#pragma config CPD = OFF        // EEPROP code protection なし

#include <xc.h>

// I/Oポートのマッピング
// 14pinのマッピング A port=|x|x|A5|A4||A3|A2|A1|A0|, B port=|x|x|C5|C4|C3|C2|x|x|
#define REG_MAP_IN			(PORTA&0b111111)
#define REG_MAP_IN_B		(PORTC&0b111100)
#define REG_MAP_DIR(x)		{TRISA=(x)&0b111111;}
#define REG_MAP_DIR_B(x)	{TRISC=0b11|((x)&0b111100);}	// RC0/1はI2Cで使用のため常にIN
#define REG_MAP_PU(x)		{WPUA=(x)&0b111111;}
#define REG_MAP_PU_B(x)		{WPUC=(x)&0b111100;}	
#define REG_MAP_LAT(x)		{LATA=(x)&0b111111;}
#define REG_MAP_LAT_B(x)	{LATC=(x)&0b111100;}

// ポート初期化
void InitPic(void)
{
  OSCCON1 = 0b0000001; /// 32MHz/2=16MHz
	PORTA = 0;
	TRISA = 0b111111;
	WPUA = 0;	
	ANSELA = 0;		// アナログ入力OFF
	LATA = 0;
	PORTC = 0;
	TRISC = 0b111111;
	ANSELC = 0;		// アナログ入力OFF
	LATC = 0;
}

//////////////////////////////////////////////////////////////////////////////
// エミュレートするチップ毎の設定
//
#ifdef EMU_MCP23008
	// MCP230008
	#define REG_IODIR	0x00
	#define REG_GPIO	0x09
	#define REG_OLAT	0x0a
	#define REG_GPPU	0x06
#else
	// MCP230017
	#define REG_IODIR	0x00
	#define REG_GPIO	0x12
	#define REG_OLAT	0x14
	#define REG_GPPU	0x0c
	#define REG_IODIR_B	0x01
	#define REG_GPIO_B	0x13
	#define REG_OLAT_B	0x15
	#define REG_GPPU_B	0x0d
#endif

// I/O Register
volatile byte Iodir = 0xff;	// Reset時すべて入力
volatile byte Olat = 0;
volatile byte Gppu = 0;		// Reset時プルアップなし
// B port for 017
volatile byte IodirB = 0xff;	// Reset時すべて入力
volatile byte OlatB = 0;
volatile byte GppuB = 0;		// Reset時プルアップなし

volatile byte Adr = 0;		// レジスタアドレス

//////////////////////////////////////////////////////////////////////////////
// レジスタ読み書き処理
//
void RegWrite(byte d)
{
	switch(Adr){
		case REG_IODIR:
			Iodir = d;
			REG_MAP_DIR(d);
			break;
		case REG_GPIO:  // 出力時はLATと同じ
		case REG_OLAT:
			Olat = d;
			REG_MAP_LAT(d);
			break;
		case REG_GPPU:
			Gppu = d;
			REG_MAP_PU(d);
			break;
#ifdef EMU_MCP23017
		case REG_IODIR_B:
			IodirB = d;
			REG_MAP_DIR_B(d);
			break;
		case REG_GPIO_B:  // 出力時はLATと同じ
		case REG_OLAT_B:
			OlatB = d;
			REG_MAP_LAT_B(d);
			break;
		case REG_GPPU_B:
			GppuB = d;
			REG_MAP_PU_B(d);
			break;
#endif
	}
}
byte RegRead()
{
	switch(Adr){
		case REG_IODIR:
			return Iodir;
		case REG_GPIO:
			return REG_MAP_IN;
		case REG_OLAT:
			return Olat;
		case REG_GPPU:
			return Gppu;
#ifdef EMU_MCP23017
		case REG_IODIR_B:
			return IodirB;
		case REG_GPIO_B:
			return REG_MAP_IN_B;
		case REG_OLAT_B:
			return OlatB;
		case REG_GPPU_B:
			return GppuB;
#endif
	}
	return 0;	// サポート外レジスタのときは0を返す
}

//////////////////////////////////////////////////////////////////////////////
// 割り込み処理 I2C
//
void interrupt intr(void)
{
	static byte f_adr=0;	// レジスタアドレスを受信中のとき1

	if(SSP1IF == 1){	// I2C割込 
		if(R_nW == 0){	// 書き込み受信 マスターtoスレーブ
			if(D_nA == 0){		// I2C Address受信
				byte temp = SSPBUF;	// BFクリアのためにダミー読み出し
				f_adr=1;			// レジスタアドレス受信中
			}else{
				if(f_adr == 1){
					Adr = SSPBUF;		// レジスタアドレス受信
					f_adr = 0;
				}else{
					RegWrite(SSPBUF);	// レジスタデータ受信処理、ACK自動送出
					Adr++;				// シーケンシャルモードなので+1する
				}
			}
		}else{		// 読み出し受信 スレーブtoマスター
			byte temp = SSPBUF;	// BFクリアのためにダミー読み出し
			CKP = 0;		// 念のためクロックストレッチする（自動でなっているはず）
			SSPBUF = RegRead();		// ここでマスターへ送るデータを用意
			Adr++;					// シーケンシャルモードなので+1する
		}
		SSP1IF = 0;		// 割込フラグクリア
		CKP = 1;		// SCL解放
	}
	// I2Cエラー処理（フラグをリセットするだけ）
	if(WCOL ==1) WCOL = 0;		// I2Cコリジョン
	if(SSPOV ==1) SSPOV = 0;	// I2Cオーバーフロー
}
////////////////////////////////////////////////////////////////////////////////
// I2Cスレーブの設定
//
void InitI2C_Slave(int address)
{
	SSPSTAT = 0;			// I2Cデフォルトの設定
	SSPCON1 = 0b00110110;	// I2Cスレーブ、7bitアドレス start/stop割り込みなし
	SEN = 1;				// クロックストレッチ有効（スレーブtoマスターの時必要)
	SSPADD = address;		// I2Cアドレスの設定(1bit左シフト済)
	SSP1IE = 1;			// I2C割込許可
	SSP1IF = 0;			// I2C割込フラグリセット
	PEIE = 1;
	GIE = 1;
}
////////////////////////////////////////////////////////////////////////////////
// メイン：初期設定のみ
//
void main(void)	
{
	InitPic();	// 初期設定
	InitI2C_Slave(0x40); // 7bitアドレスは0x20
	for(;;);
}
