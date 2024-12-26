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

#define LED_PIN 0     // MCP23XXX pin LED is attached to

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
  mcp.pinMode(LED_PIN, OUTPUT);

  Serial.println("Looping...");
}

void loop() {
  mcp.digitalWrite(LED_PIN, HIGH);
  delay(500);
  mcp.digitalWrite(LED_PIN, LOW);
  delay(500);
}
