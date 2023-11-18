# コンデンサ静電容量測定

参考にしているのはこちら。

https://101010.fun/iot/arduino-measure-capacitance.html

まずは部品の確認。記事では測定用に 2MΩ, 1kΩを放電用に用いている。

放電に関しては、こちらで計算できる。

https://www.digikey.jp/ja/resources/conversion-calculators/conversion-calculator-capacitor-safety-discharge

手持ちの一番大きいコンデンサが 0.1μF。充電電圧を 5V で 0.01V まで落としてみる。0.01V まで
落ちれば、どうせ自分には測定できない。

10kΩで 0.006秒。まあとりあえずこれで OK でしょう。

時定数はこちらのサイトで計算できる。

https://www.digikey.jp/ja/resources/conversion-calculators/conversion-calculator-time-constant

手持ち 10kΩで 0.1μF の時定数は 0.001 sec. 16MHz 駆動だから 1万6千サイクル分なわけだけど、
どうなんだろうか。

参考サイトの方は、100pF も精度良かったとのことだが、この場合時定数は 0.0002 sec であり、自
分の手持ちよりも短い。

ということで、手持ちの 10kΩの抵抗でとりあえずやってみることにしよう。

--> 結果はあまり良くない。次のようにして測定まで何ループできてるのか調べてみたが、
    
```
  unsigned int i=0;

  while ( analogRead(ANALOG_READ_PIN) < 647)
  {
      i=i+1;
  }; //1023*64.2=646.5。

  double T = micros() - time_start; // T: 時定数
    Serial.print(i);
    Serial.print(", ");
    Serial.println(T);
```

この while が、わずか 13
ループしかない。もちろん演算なんかしなければもう少し増えるとは思うが、どのみち、10^1
のオーダーであることには変わりがない。 Arduino
は自分で思ってたよりも動作が遅いのだな。

なのであくまで壊れているかどうかの判定程度である。ちゃんとやるときは、時定数 0.1 - 1.0 sec
あたりを狙うのが良いのではないだろうか。

ソースはこちら

![ソース](./arduino/main/main.ino)
