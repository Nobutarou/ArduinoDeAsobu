#define cbi(addr, bit) addr &= ~(1 << bit)  // addrのbit目を'0'にする
#define sbi(addr, bit) addr |= (1 << bit)   // addrのbit目を'1'にする
#define _PULSE_PIN 2

// シリアル通信について TinkerCad と実物で相違がある。
// Linux の改行が <LF><CR> だかで、文字+Enter という送信になってるのかも
// まあ Windows 使わないからいいや

const char PULSE_PIN = 2;
const char ANALOG_READ_PIN = A5;

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2^32-1); //no timeout
  pinMode( PULSE_PIN , OUTPUT);
  digitalWrite(PULSE_PIN, LOW);

  // PIC と違い レジスタ名bits.ビット名での操作はできない。
  // & は 1&1=1, 1&0=0, 0&1=0, 0&0=0 
  // まず上位 5ビットを保持したまま、下位3ビットをクリア
  ADCSRA = ADCSRA & 0b11111000;

  // 第2 ビットを 1 に変える。
  // | or は 1&1=1, 1&0=1, 0&1=1, 0&0=0 だから、第2 ビット以外は元の値が保持される
  ADCSRA = ADCSRA | 0b00000010;

}


void discharge() { 
  digitalWrite(PULSE_PIN, LOW);
  delay(1000);
}

unsigned long charge() {
//  digitalWrite(PULSE_PIN, HIGH);
  sbi(PORTD, PORTD2);
  return micros();
}

void loop() {
  double _R;
  _R=r_setup();

  // TinkerCad では文字を送る必要があったが、実物では Enter で反応する
  Serial.println("Enter to reset registor");

  // TinkerCad では不要だったけど、これがないと Serial.available()>0 になってしまう。
  Serial.end();
  Serial.begin(9600);
  
  while(Serial.available()==0){
    discharge();

    unsigned long time_start = charge();

    while ( analogRead(ANALOG_READ_PIN) < 647){};//1023*64.2=646.5。

    double T = micros() - time_start; // T: 時定数
    double c=T/_R; // [uF]
  
    Serial.println(c,16);
    digitalWrite(PULSE_PIN, LOW);
  }
  Serial.end();
  Serial.begin(9600);
}

double r_setup()
{
  double registor;
  double x; // general purpose
  short i; //
  char a; // general purpose
  
  Serial.println("Input the registor [Ohm]");
  Serial.println("Scientific notation only (ex, 1.23e-4)");

  while (Serial.available() == 0) {};
  x=Serial.parseFloat();
  a=Serial.read();
  i=Serial.parseInt();
  
  registor=x*pow(10.0,i);
  
  Serial.print("Registor: ");
  Serial.print(registor,16);
  Serial.println(" Ohm");
  return registor;
}
