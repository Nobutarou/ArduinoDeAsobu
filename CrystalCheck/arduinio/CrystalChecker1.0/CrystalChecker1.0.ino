// ASOBoard D2 にカウンタの QH を繋ぐ
// 1s の間 FALLING をカウントする 

unsigned long count = 0;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  unsigned long final_count; 

  Serial.begin(9600);
  Serial.println("Enter to start count");
  while (Serial.available() == 0) {}; //打ち込みを待つ
  Serial.read();
  Serial.end();

  pinMode(2,INPUT);
  delay(100);
  attachInterrupt(digitalPinToInterrupt(2),falling,FALLING);
  delay(1000);
  final_count = count; //もたついてる間に count が進むのが嫌だ
  detachInterrupt(digitalPinToInterrupt(2)); //念の為止めておく

  Serial.begin(9600);
  Serial.print("Count: ");
  Serial.println(final_count);
  Serial.print("Freq [MHz]: ");
  Serial.println(final_count*256.0/1.0e6);

//  Serial.println("Enter to restart");
//  while (Serial.available() == 0) {}; //打ち込みを待つ
//  Serial.read();
//  Serial.println("Restarting.");
  Serial.end();
  resetFunc();
}

void loop()
{
}

void falling()
{
  count++;
}

