// C++ code
#define VA 4 // typeA V
#define DMA 5 // typeA D-
#define DPA 6 // typeA D+
#define GA 7 // typeA GND
#define SA 8 // typeA shield
#define VB 13 // typeB V
#define DMB 12 // typeB D-
#define DPB 11 // typeB D+
#define GB 10// typeB GND
#define SB 9 // typeB shield

void(* resetFunc) (void) = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Plug in a USB cable. Enter to start check");
  while (Serial.available() == 0) {};
  Serial.read();

  ResetAllPins();

  // typeA V to typeB all
  pinMode(VA, OUTPUT);
  digitalWrite(VA, HIGH);
  delay(1);
  if(digitalRead(VB)){
    Serial.println("VA-VB: OK");
  } else {
    Serial.println("VA-VB: ERR, No connection");
  }; 
  if(digitalRead(DMB)){
    Serial.println("VA-D-B: ERR, wrongly connected");
  }; 
  if(digitalRead(DPB)){
    Serial.println("VA-D+B: ERR, wrongly connected");
  }; 
  if(digitalRead(GB)){
    Serial.println("VA-GNDB: ERR, wrongly connected");
  }; 
  if(digitalRead(SB)){
    Serial.println("VA-ShieldB: ERR, wrongly connected");
  }; 
  
  ResetAllPins();
  
  // typeA D- to typeB all
  pinMode(DMA, OUTPUT);
  digitalWrite(DMA, HIGH);
  delay(1);
  if(digitalRead(VB)){
    Serial.println("D-A-VB: ERR, wrongly connected");
  };
  if(digitalRead(DMB)){
    Serial.println("D-A-D-B: OK");
  } else {
    Serial.println("D-A-D-B: Warn, No connection");
  };    
  if(digitalRead(DPB)){
    Serial.println("D-A-D+B: ERR, wrongly connected");
  };
  if(digitalRead(GB)){
    Serial.println("D-A-GB: ERR, wrongly connected");
  };
  if(digitalRead(SB)){
    Serial.println("D-A-ShieldB: ERR, wrongly connected");
  };
 
  ResetAllPins();
  
  // typeA D+ to typeB all
  pinMode(DPA, OUTPUT);
  digitalWrite(DPA, HIGH);
  delay(1);
  if(digitalRead(VB)){
    Serial.println("D+A-VB: ERR, wrongly connected");
  };
  if(digitalRead(DMB)){
    Serial.println("D+A-D-B: ERR, wrongly connected");
  };
  if(digitalRead(DPB)){
    Serial.println("D+A-D+B: OK");
  } else {
    Serial.println("D+A-D+B: Warn, No connection");
  };    
  if(digitalRead(GB)){
    Serial.println("D+A-GNDB: ERR, wrongly connected");
  };
  if(digitalRead(SB)){
    Serial.println("D+A-ShieldB: ERR, wrongly connected");
  };
 
  ResetAllPins();
  
  // typeA GND to typeB all
  pinMode(GA, OUTPUT);
  digitalWrite(GA, HIGH);
  delay(1);
  if(digitalRead(VB)){
    Serial.println("GNDA-VB: ERR, wrongly connected");
  };
  if(digitalRead(DMB)){
    Serial.println("GNDA-D-B: ERR, wrongly connected");
  };
  if(digitalRead(DPB)){
    Serial.println("GNDA-D+B: ERR, wrongly connected");
  };
  if(digitalRead(GB)){
    Serial.println("GNDA-GNDB: OK");
  } else {
    Serial.println("GNDA-GNDB: ERR, No connection");
  };
  // シールドは最後にまとめて

  ResetAllPins();
  
  // typeA Shiled to typeB all
  pinMode(SA, OUTPUT);
  digitalWrite(SA, HIGH);
  delay(1);
  if(digitalRead(VB)){
    Serial.println("ShiledA-VB: ERR, wrongly connected");
  };
  if(digitalRead(DMB)){
    Serial.println("ShieldA-D-B: ERR, wrongly connected");
  };
  if(digitalRead(DPB)){
    Serial.println("ShieldA-D+B: ERR, wrongly connected");
  };
  if(digitalRead(SB)){
    Serial.println("Shiled: Yes");
  } else {
    Serial.println("Shiled: No");
  };
  if(digitalRead(GB)){
    Serial.println("Shiled is grounded in the cable");
  } ;

  Serial.end();
  resetFunc();
}

void loop()
{
}

void ResetAllPins()
{
  // change all pins to input
  pinMode(VA, INPUT);
  pinMode(DMA, INPUT);
  pinMode(DPA, INPUT);
  pinMode(GA, INPUT);
  pinMode(SA, INPUT);
  pinMode(VB, INPUT);
  pinMode(DMB, INPUT);
  pinMode(DPB, INPUT);
  pinMode(GB, INPUT);
  pinMode(SB, INPUT);
  
}