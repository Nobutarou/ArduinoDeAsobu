// ASOBoard D2 にカウンタの QH を繋ぐ
// 1s の間 FALLING をカウントする 

unsigned long count = 0;

void setup()
{
  unsigned long final_count; 

  pinMode(2,INPUT);
  delay(100);
  attachInterrupt(digitalPinToInterrupt(2),falling,FALLING);
  delay(2000);

  final_count = count; //もたついてる間に count が進むのが嫌だ

  Serial.begin(9600);
  Serial.print("Count: ");
  Serial.println(count);
  Serial.print("Freq [MHz]: ");
  Serial.println(count*256.0/1.0e6);
  
}

void loop()
{
}

void falling()
{
  count++;
}

