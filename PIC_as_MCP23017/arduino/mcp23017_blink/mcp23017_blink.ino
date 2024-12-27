//#include <mcp23017.h>
//
//#define MCP_ADDR (0)
//
//MCP23017 mcp(MCP_ADDR);
//
//void setup(void)
//{
//    mcp.start();
//    mcp.pinMode(GPB5, OUTPUT);
//}
//
//void loop(void)
//{
//    mcp.digitalWrite(GPB5, HIGH);
//    delay(250);
//    mcp.digitalWrite(GPB5, LOW);
//    delay(250);
//}

#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>

//#define LED_PIN 0     // MCP23XXX pin LED is attached to

// only used for SPI
#define CS_PIN 6

// uncomment appropriate line
//Adafruit_MCP23X08 mcp;
Adafruit_MCP23X17 mcp;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("MCP23xxx Blink Test!");

  // uncomment appropriate mcp.begin
  if (!mcp.begin_I2C()) {
  //if (!mcp.begin_SPI(CS_PIN)) {
    Serial.println("Error.");
    while (1);
  }

  // configure pin for output
  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(8, OUTPUT);
  mcp.pinMode(9, OUTPUT);
  mcp.pinMode(10, OUTPUT);
  mcp.pinMode(11, OUTPUT);
  mcp.pinMode(12, OUTPUT);
  mcp.pinMode(13, OUTPUT);

  Serial.println("Looping...");
}

void loop() {
  mcp.digitalWrite(13, LOW);
  mcp.digitalWrite(0, HIGH);
  delay(100);
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, HIGH);
  delay(100);
  mcp.digitalWrite(1, LOW);
  mcp.digitalWrite(2, HIGH);
  delay(100);
  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(3, HIGH);
  delay(100);
  mcp.digitalWrite(3, LOW);
  mcp.digitalWrite(8, HIGH);
  delay(100);
  mcp.digitalWrite(8, LOW);
  mcp.digitalWrite(9, HIGH);
  delay(100);
  mcp.digitalWrite(9, LOW);
  mcp.digitalWrite(10, HIGH);
  delay(100);
  mcp.digitalWrite(10, LOW);
  mcp.digitalWrite(11, HIGH);
  delay(100);
  mcp.digitalWrite(11, LOW);
  mcp.digitalWrite(12, HIGH);
  delay(100);
  mcp.digitalWrite(12, LOW);
  mcp.digitalWrite(13, HIGH);
  delay(100);
}
