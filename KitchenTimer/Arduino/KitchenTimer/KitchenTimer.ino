#define _C 262
#define _D 294
#define _E 330
#define _F	349
#define _G	392
#define _A 440
#define _B 494
#define _CH 523
#define _haku 250

void setup()
{
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
//  Serial.begin(9600);

  char D4=digitalRead(4);
  char D5=digitalRead(5);
  char D6=digitalRead(6);
  char D7=digitalRead(7);
  unsigned long t=(D7<<3)+(D6<<2)+(D5<<1)+D4;
//  Serial.println(t*60*1000);
  
  delay(t*60*1000);
//  delay(60000);
  melody();

}

void loop()
{

}

void melody()
{
  tone(2, _C);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _E);
  delay(_haku);
  tone(2, _F);
  delay(_haku);

  tone(2, _G);
  delay(_haku*2);
  tone(2, _CH);
  delay(_haku*2);
  
  tone(2, _A);
  delay(_haku*2);
  tone(2, _G);  
  delay(_haku);
  tone(2, _F);  
  delay(_haku);
  
  tone(2, _G);
  delay(_haku*2);
  noTone(2);
  delay(_haku*2);

  tone(2, _F);
  delay(_haku*2);
  tone(2, _E);
  delay(_haku);
  tone(2, _D);
  delay(_haku);

  tone(2, _E);
  delay(_haku*2);
  tone(2, _C);
  delay(_haku*2);

  tone(2, _D);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _E);
  delay(_haku);

  tone(2, _D);
  delay(_haku*2);
  noTone(2);
  delay(_haku*2);

  tone(2, _C);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _E);
  delay(_haku);
  tone(2, _F);
  delay(_haku);

  tone(2, _G);
  delay(_haku*2);
  tone(2, _CH);
  delay(_haku*2);
  
  tone(2, _A);
  delay(_haku*2);
  tone(2, _G);  
  delay(_haku);
  tone(2, _F);  
  delay(_haku);
  
  tone(2, _G);
  delay(_haku*2);
  noTone(2);
  delay(_haku*2);

  tone(2, _F);
  delay(_haku*2);
  tone(2, _E);
  delay(_haku);
  tone(2, _D);
  delay(_haku);

  tone(2, _E);
  delay(_haku*2);
  tone(2, _C);
  delay(_haku*2);

  tone(2, _D);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _D);
  delay(_haku);
  tone(2, _E);
  delay(_haku);

  tone(2, _C);
  delay(_haku*2);
  noTone(2);
  delay(_haku*2);
}
