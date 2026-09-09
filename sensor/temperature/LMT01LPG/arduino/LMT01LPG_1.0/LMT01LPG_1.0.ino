#define VP 3 // VP ピン
#define VN 2 // VN ピン
#define LED1 A1 // 温度 1桁目
#define LED2 A2 // 温度 2桁目
#define LED3 A3 // 温度 3桁目
#define LED4 A4 // 温度 4桁目
#define LED5 A5 // 温度 5桁目

unsigned int count;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  pinMode(VP, OUTPUT);
  pinMode(VN, INPUT);

//  Serial.begin(9600);
//  Serial.println("Enter to start count");
//  while (Serial.available() == 0) {}; //打ち込みを待つ
//  Serial.read();
//  Serial.end();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

void loop()
{
  unsigned int final_count; 
  double temp;
  byte i;

  count = 0;

  // digitalWrite は数ms 掛かるようだが、十分に余裕はある
  digitalWrite(VP, HIGH);
  attachInterrupt(digitalPinToInterrupt(VN),falling,FALLING);
  delay(104);
  final_count = count; //もたついてる間に count が進むのが嫌だ
  detachInterrupt(digitalPinToInterrupt(2)); //念の為止めておく
  digitalWrite(VP, LOW);

  temp=(final_count/4096.0*256.0)-50.0;

  Serial.begin(9600);
  Serial.print(temp, 2);
  Serial.println(" deg C");
  Serial.end();

  digitalWrite(LED1, ( (int)temp & 1) );
  digitalWrite(LED2, ( ( (int)temp >> 1) & 1) );
  digitalWrite(LED3, ( ( (int)temp >> 2) & 1) );
  digitalWrite(LED4, ( ( (int)temp >> 3) & 1) );
  digitalWrite(LED5, ( ( (int)temp >> 4) & 1) );

  delay(1000);

}

void falling()
{
  count++;
}

