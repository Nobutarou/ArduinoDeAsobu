void setup() {
  Serial.begin(9600);

  //マイコンが動いていることを確認するため
  pinMode(LED_BUILTIN, OUTPUT); 

  pinMode(PIN_PC0,OUTPUT);
  pinMode(PIN_PC1,INPUT);
  digitalWrite(PIN_PC0,HIGH);
}

void loop() {
  //マイコンが動いていることを確認するため
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println(digitalRead(PIN_PC1));

  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println(digitalRead(PIN_PC1));

  delay(1000);


  // 結果
  // | C0-C1 抵抗[Ω] | C0電圧(電圧計)[V]| C1電圧(電圧計)[V]| digitalRead(PIN_PC1) |
  // | ---            | ---              | ----             |  ---                 |
  // | 無し           | 5                | 0                | 0 or 1 **1           |
  // | 10M            | 5                | 0                | 1 or 0 **2           |
  // |  1M            | 5                | かすかに動く     | 1 or 0 **2           |
  // | 100k           | 5                | 1.5              | 1 or 0 **2           |
  // |  10k           | 5                | 4                | 1                    |
  // | 2.7k           | 5                | 4.6              | 1                    |
  // | 1.5k           | 5                | 4.8              | 1                    |
  // |   1k           | 5                | 4.8              | 1                    |
  // | 300            | 5                |  5               | 1                    |
  //
  // **1) 電圧計を C0 に当てたりリセットしたりすると変わる
  // **2) 電圧計を C0 当てないと 1, 当てると 0
}

