#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

#define RX1 19
#define TX1 18 // RX, TX
#define MAX485_DE 53
#define MAX485_RE_NEG 52

ModbusMaster node;
void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}
void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.println("start init serial 0");
  Serial.begin(9600);

  while (!Serial) {
  Serial.println("loop for init serial 0");
  }
  Serial.println("start init software serial");
  Serial1.begin(9600);

  while (!Serial1) {
  Serial.println("loop for init software serial");
  }
  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}
void loop()
{
  uint8_t result;
  uint16_t data[2];
  Serial.println("get data");
  result = node.readInputRegisters(1, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Temp: ");
    Serial.println(node.getResponseBuffer(0)/10.0f);
    Serial.print("Humi: ");
    Serial.println(node.getResponseBuffer(1)/10.0f);
    Serial.println();
  }
 delay(1000);
}
