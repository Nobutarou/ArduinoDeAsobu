// VDD - 2.7kohm - PC0
// VDD - 2.7kohm - PD1
// Other pins are floating

void setup()
{
  Serial.begin(9600);
  analogReadResolution(12);
}

void loop()
{
  Serial.print("PC0=");
  Serial.println(analogRead(PIN_PC0));

  // これをコメントアウトするかしないかで結果が変化する。
  //Serial.print("PD1=");
  //Serial.println(analogRead(PIN_PD1));

  //Serial.print("PA6=");
  //Serial.println(analogRead(PIN_PA6));

  // fuse6 SYSCFG1 の MVSYSCFG を "0b10" にすることで動くようになった。

}
