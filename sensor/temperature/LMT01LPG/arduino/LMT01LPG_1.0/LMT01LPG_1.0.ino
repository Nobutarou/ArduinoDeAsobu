#define VP 3 // VP ピン
#define VN 2 // VN ピン

unsigned int count;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  pinMode(VP, OUTPUT);
  pinMode(VN, INPUT);


  Serial.begin(9600);
  Serial.println("Enter to start count");
  while (Serial.available() == 0) {}; //打ち込みを待つ
  Serial.read();
  Serial.end();
}

void loop()
{
  unsigned int final_count; 
  double temp;

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

  delay(1000);

}

void falling()
{
  count++;
}

