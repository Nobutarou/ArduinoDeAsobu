// ASOBoard D2 にカウンタの 

byte state = 0;
unsigned int count = 0;

void setup()
{
  pinMode(2,INPUT);
  attachInterrupt(digitalPinToInterrupt(2),blink,FALLING);
  delay(10000);
  Serial.begin(9600);
  Serial.print("Count: ");
  Serial.println(count);
  Serial.print("Freq [Hz]: ");
  Serial.println(count/10.0);
  
}

void loop()
{
}

void blink()
{
  state = !state;
  digitalWrite(13,state);
  count++;
}

// 割り込みに使えるのは D2 or D3

// こんな動作でどうだろうか

// いきなり水晶を発振させてカウンタも動かす
// カウンタ 8番目のピンが FALLING のたびにカウント
// 1秒待ってカウント数と周波数発表
// 1秒って MHz から見たらとんでもなく長いので開始時点のずれは誤差

// あとは Arduino が 16MHz/256 での FALLING を検知できるかどうかだけ

