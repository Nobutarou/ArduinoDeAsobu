// MCP23017 関連
// 左に1ビットシフト済み
#define MCP23017_ADDRESS 0b01000000
// Input, Output を指定するレジスタ
#define MCP23017_IODIRA  0b00000000
#define MCP23017_IODIRB  0b00000001
// High, Low を指定するレジスタ
#define MCP23017_GPIOA   0x12
#define MCP23017_GPIOB   0x13

// MCP23017 
volatile uint8_t _gpioa; // MCP23017 の GPIOA レジスタの状態
volatile uint8_t _gpiob; // MCP23017 の GPIOB レジスタの状態
void OreLCD_I2C_Send(uint8_t _reg, uint8_t _data); //MCP23017専用なので I2C の方には入れない
                                                //ことにした

// ピン配置
// RS = B0 (PIC ではなくて GPIO で記述)
// RW = B5
// EN = B4
// D4 = B2
// D5 = B3
// D6 = B1
// D7 = A2

void OreLCDBegin(); //初期化、最初に呼ぶ
void OreLCD_RSWrite(uint8_t bit); // bit 操作
void OreLCD_RWWrite(uint8_t bit);
void OreLCD_ENWrite(uint8_t bit);
void OreLCD_DB7Write(uint8_t bit);
void OreLCD_DB6Write(uint8_t bit);
void OreLCD_DB5Write(uint8_t bit);
void OreLCD_DB4Write(uint8_t bit);
void OreLCD_EN_PULSE(); // パルス。確定処理
void OreLCDWriteByte(uint8_t byte); // 4ビットモードで 8ビットの送信
void OreLCD_CLS(); // 表示クリア
void OreLCD_Print(char* str, uint8_t len); //漢らしく文字列以外は受けつけない。自分で文字列を作れ
void OreLCD_Locate(uint8_t line, uint8_t column);
void OreLCD_Char(uint8_t code);// 文字コードを直送
void OreLCD_Float(float val, uint8_t num);// 不動点少数を表示
void OreLCD_Uint16(uint16_t num); // 16bit の正の整数を表示

/***********************************************************************************************************************
* Function Name: OreUART1_Send_U16
* Description  : 16ビットの正の整数を 10進数で表示
* Arguments    : num - 16ビットの正の整数
* Return Value : None
***********************************************************************************************************************/
void OreLCD_Uint16( uint16_t num)
{
/* {{{ */
  uint8_t b; // その桁の値
  uint8_t flag=0; // 最初の桁を見つけるフラグ

  // 10000 で割る。答が 0 ならスキップ
  flag=flag | (uint8_t)(num/10000); 
  if(flag != 0){
    b=(uint8_t)(num/10000)+48;
    OreLCD_Char(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%10000;
  };

  flag=flag | (uint8_t)(num/1000); 
  if(flag != 0){
    b=(uint8_t)(num/1000)+48;
    OreLCD_Char(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%1000;
  };

  flag=flag | (uint8_t)(num/100); 
  if(flag != 0){
    b=(uint8_t)(num/100)+48;
    OreLCD_Char(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%100;
  };

  flag=flag | (uint8_t)(num/10); 
  if(flag != 0){
    b=(uint8_t)(num/10)+48;
    OreLCD_Char(b);
//    OreUART1_Send_Txt( (unsigned char *)&b, 1);
    num=num%10;
  };

  // 最後は 0 でもなんでも表示するのでそのまま送る。
  num=num%10+48;
  OreLCD_Char( (uint8_t)num);
//  OreUART1_Send_Txt( (unsigned char *)&num, 1);
/* }}} */
}

/***********************************************************************************************************************
* Function Name: OreLCD_Float
* Description  : Float 型を指数形式で表示
* Arguments    : val - float の実数
*              : num - 数字部分の小数点以下の文字数 1.23e-3 なら2 
* Return Value : None
***********************************************************************************************************************/
void OreLCD_Float( float val, uint8_t num)
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
    OreLCD_Char(0b00101101); //-
  };
  
  //最初の桁を出力
  // 1.23 みたいなら 1 って出るはず。
  OreLCD_Char( (uint8_t)val + 0b00110000); //0b0011000 が "0"
  OreLCD_Char( 0b00101110); // dot 

  val=(float)(val- (uint8_t)val); //val=1.23 だったなら val=0.23 のはず

  // 残りの桁を出そう
  for(i=0;i<num;i++){
    val=val * 10.0; //0.23 なら 2.3
    OreLCD_Char( (uint8_t)val + 0b00110000); //0b0011000 が "0"
    val=(float)(val- (uint8_t)val); //2.3 → 0.3 になるはず
  };

  // "e" を出力
  OreLCD_Char( 0b01100101); //e

  // 負なら "-" を出す
  if(keta_sign==1){
    OreLCD_Char(0b00101101); //-
  };

  // 指数値を出す
  OreLCD_Uint16( (uint16_t) keta);

  return;
/* }}} */
}

void OreLCD_Char(uint8_t code)
{
{{{
  OreLCD_RSWrite(1);
  OreLCDWriteByte(code);
  OreLCD_RSWrite(0);
}}}
}

// <string.h> の strlen は使ってもプログラムサイズにほぼ影響ないから
// 引数 len を省いて自分で求めさせても良いかも
void OreLCD_Print(char* str, uint8_t len)
{
/* {{{ */
  uint8_t i; //汎用 
  if(len==0){
    return;
  }

  OreLCD_RSWrite(1);

  for(i=0; i<len; i++){
    OreLCDWriteByte( (uint8_t)str[i]);
  }

  OreLCD_RSWrite(0);
/* }}} */
}

// line: 一行目 0, 二行目 1
// column: 行頭 0
void OreLCD_Locate(uint8_t line, uint8_t column)
{
{{{
  // カーソル位置 = DD RAM アドレス
  // |RW|RW|DB7-DB0| = |0|0|1xxx xxxx|
  // 一行目は |DB7-DB0| は |1000 0000| ~ |1010 1111| (40文字の LCD と共通なんだと思う)
  // 二行目は |DB7-DB0| は |1100 0000| ~ !1100 0111|
  OreLCDWriteByte( 0b10000000+(uint8_t)(line<<6)+column );
}}}
}

void OreLCDBegin()
{
/* {{{ */
  // MCP23017 初期化   

  // MCP23017 の両ポートを Output に
  OreLCD_I2C_Send(MCP23017_IODIRA, 0);
  OreLCD_I2C_Send(MCP23017_IODIRB, 0);

  // 全ポートを Low に
  _gpioa=0;
  _gpiob=0;
  OreLCD_I2C_Send(MCP23017_GPIOA, _gpioa);
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
  //while(1){};
  //全部 low だった

  // LCD 初期化

  // VDD が 4.5V に到達してから 15ms 以上待つ
  // 270kHz の話と仮定して 100kHz であれば 270/100*15=40.5ms
  // 区切の良いところで
  R_BSP_SoftwareDelay(47, BSP_DELAY_MILLISECS);

  // |RS|RW|DB7-DB4| = |0|0|0011|
  OreLCD_RSWrite(0);
  OreLCD_RWWrite(0);
  OreLCD_DB7Write(0);
  OreLCD_DB6Write(0);
  OreLCD_DB5Write(1);
  OreLCD_DB4Write(1);

  OreLCD_EN_PULSE();

  // 4.1ms@270kHz = 11.07ms@100kHz → 22ms 待つ
  R_BSP_SoftwareDelay(22, BSP_DELAY_MILLISECS);

  // |RS|RW|DB7-DB4| = |0|0|0011|
  // 変化してないから操作不要
  //OreLCD_RSWrite(0);
  //OreLCD_RWWrite(0);
  //OreLCD_DB7Write(0);
  //OreLCD_DB6Write(0);
  //OreLCD_DB5Write(1);
  //OreLCD_DB4Write(1);
  OreLCD_EN_PULSE();

  // 100us@270kHz 待つ。
  // 270us@100kHz → 470us
  R_BSP_SoftwareDelay(470, BSP_DELAY_MICROSECS);

  // |RS|RW|DB7-DB4| = |0|0|0011|
  // 変化してないから操作不要
  //OreLCD_RSWrite(0);
  //OreLCD_RWWrite(0);
  //OreLCD_DB7Write(0);
  //OreLCD_DB6Write(0);
  //OreLCD_DB5Write(1);
  //OreLCD_DB4Write(1);
  OreLCD_EN_PULSE();

  // |RS|RW|DB7-DB4| = |0|0|0010|
  //OreLCD_RSWrite(0);
  //OreLCD_RWWrite(0);
  //OreLCD_DB7Write(0);
  //OreLCD_DB6Write(0);
  //OreLCD_DB5Write(1);
  OreLCD_DB4Write(0); //変化はここだけ
  OreLCD_EN_PULSE();

  // |RS|RW|DB7-DB0| = |0|0|0010 1000| = 2Line 
  // 4 ビットモードなので
  // |RS|RW|DB7-DB4| = |0|0|0010|
  // |RS|RW|DB7-DB4| = |0|0|1000|
  // の2回送るのを OreLCDWriteByte() にやらせる
  OreLCDWriteByte(0b00101000);
  
  // |RS|RW|DB7-DB0| = |0|0|0000 1000| = 表示オフ
  OreLCDWriteByte(0b00001000);

  // |RS|RW|DB7-DB0| = |0|0|0000 0001| = 表示クリア
  // 時間の掛かる処理なので、別の関数とする
  OreLCD_CLS();
  // |RS|RW|DB7-DB0| = |0|0|0000 0110| = カーソルは文字の右、文字ではなくカーソルが動く
  OreLCDWriteByte(0b00000110);

  // 初期化完了

  // さらに一度表示を on にしてクリアしておく
  // |RS|RW|DB7-DB0| = |0|0|0000 1100|  = 表示オン
  OreLCDWriteByte(0b00001100);

  OreLCD_CLS();
/* }}} */
}

void OreLCD_CLS()
{
/* {{{ */
  OreLCD_RSWrite(0);
  OreLCD_RWWrite(0);
  OreLCDWriteByte(0b00000001);
  // 1.5ms@270Khz = 4.1ms@100kHz --> 4700us
  R_BSP_SoftwareDelay(4700, BSP_DELAY_MICROSECS);

  // GCBasic にはこれを入れていた
  // 正直これが必要とは思っていない
  // DD RAM のアドレスをなんでここでセットするのか
  // 37us@270kHz = 99.9us@100kHz -> 100us
  //OreLCDWriteByte(0b10000000);
  //R_BSP_SoftwareDelay(100, BSP_DELAY_MICROSECS);
  /* }}} */
}

// これ自体は RS, RW を変更しない
void OreLCDWriteByte(uint8_t byte)
{
/* {{{ */
  //先頭の 4ビットを書き込む
  OreLCD_DB7Write(( byte >> 7 ) & 1);
  OreLCD_DB6Write(( byte >> 6 ) & 1);
  OreLCD_DB5Write(( byte >> 5 ) & 1);
  OreLCD_DB4Write(( byte >> 4 ) & 1);
  OreLCD_EN_PULSE();
  
  // GCBasic の自作ライブラリから持ってきてるんだけど、なぜここにウェイトを入れたかメモが
  // ないし、思い出せない。
  R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);

  //下位の 4ビットを書き込む
  OreLCD_DB7Write(( byte >> 3 ) & 1);
  OreLCD_DB6Write(( byte >> 2 ) & 1);
  OreLCD_DB5Write(( byte >> 1 ) & 1);
  OreLCD_DB4Write( byte  & 1);

  OreLCD_EN_PULSE();
  R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);
/* }}} */
}

// RS: GPIOB #0
void OreLCD_RSWrite(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11111110;
  } else {
    _gpiob = _gpiob | 0b00000001;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// RW: GPIOB #5
void OreLCD_RWWrite(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11011111;
  } else {
    _gpiob = _gpiob | 0b00100000;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// EN: GPIOB #4
void OreLCD_ENWrite(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11101111;
  } else {
    _gpiob = _gpiob | 0b00010000;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// DB7: GPIOA #2
void OreLCD_DB7Write(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpioa = _gpioa & 0b11111011;
  } else {
    _gpioa = _gpioa | 0b00000100;
  };
  OreLCD_I2C_Send(MCP23017_GPIOA, _gpioa);
/* }}} */
}

// DB6: GPIOB #1
void OreLCD_DB6Write(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11111101;
  } else {
    _gpiob = _gpiob | 0b00000010;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// DB5: GPIOB #3
void OreLCD_DB5Write(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11110111;
  } else {
    _gpiob = _gpiob | 0b00001000;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// DB4: GPIOB #2
void OreLCD_DB4Write(uint8_t bit)
{
/* {{{ */
  if(bit == 0){
    _gpiob = _gpiob & 0b11111011;
  } else {
    _gpiob = _gpiob | 0b00000100;
  };
  OreLCD_I2C_Send(MCP23017_GPIOB, _gpiob);
/* }}} */
}

// EN のパルス
void OreLCD_EN_PULSE()
{
/* {{{ */
  // 270kHz 駆動においてセットアップ時間 t_AS 最小は 140ns
  // 100kHz なら 270/100*140=378ns
  // R_BSP_SoftwareDelay の最小単位は us
  // なので 1us ウェイトする
  R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);
  OreLCD_ENWrite(1);
  
  // EN を掛けたあとのパルス幅の最小 PW_EH は 450ns@270kHz = 1.22us@100kHz
  // 良いところで 2us のウェイト
  R_BSP_SoftwareDelay(2, BSP_DELAY_MICROSECS);
  OreLCD_ENWrite(0);

  // 最小イネーブルサイクルは 1000ns だが、そもそも各操作に最大 37us@270kHz 掛かる。
  // なので、この段階で操作が終わるのを待つのが安全だろう
  // 37us@270kHz = 99.9us@100kHz → 100us 待つ
  R_BSP_SoftwareDelay(100, BSP_DELAY_MICROSECS);
/* }}} */
}

/***********************************************************************************************************************
* Function Name: OreLCD_I2C_Send
* Description  : 2バイト送る。子機のレジスタアドレス + 命令パターン用
* Arguments    : _reg: レジスタアドレス (1バイト)
*              : _data: 命令 (1バイト)
* Return Value : None
***********************************************************************************************************************/
void OreLCD_I2C_Send(uint8_t _reg, uint8_t _data)
{
/* {{{ */
  uint8_t OreTxBuf[2];
  //unsigned short err;
  _i2c_send_end = 0;
  OreTxBuf[0]=_reg;
  OreTxBuf[1]=_data;
  //err = R_Config_IICA0_Master_Send( MCP23017_ADDRESS, OreTxBuf, 2, 10);
  R_Config_IICA0_Master_Send( MCP23017_ADDRESS, OreTxBuf, 2, 255);
  while(_i2c_send_end == 0){}; // 送信完了を待つ
  //if(err == MD_ERROR1){
  //  //OreUART1_Send_U16(1);
  //  // I2C バスがビジーらしい
  //  while(1){};
  //} else if(err == MD_ERROR2){
  //  //OreUART1_Send_U16(2);
  //  while(1){};
  //}
  _i2c_send_end = 0;
/* }}} */
}
// vim: set filetype=c : 
