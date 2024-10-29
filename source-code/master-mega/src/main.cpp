#include <Arduino.h>
#include <SPI.h>
#include <ModbusMaster.h>

// RS485 Communication setup
#define MAX485_DE      8
#define MAX485_RE_NEG  9
#define RX1 18
#define TX1 19

ModbusMaster node;

uint8_t result;
float temp, hum;
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

void setup() {

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  // Initialize communication
  Serial.begin(9600);  // For monitoring
  Serial1.begin(9600); // RS485

  // RS485 direction control

  // Modbus node setup
  node.begin(1, Serial1);  // Modbus address 1, using Serial1 for RS485
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("Modbus communication initialized.");
}

void loop() {
  uint8_t result;
  uint16_t data;

  result = node.readHoldingRegisters(0x0000, 1);


  // Reading data from XY-MD02 over RS485
  if (result == node.ku8MBSuccess) {
    temp = node.getResponseBuffer(0) / 10.0f;
    hum = node.getResponseBuffer(1) / 10.0f;
    //    Serial.print("Temp: "); Serial.print(temp); Serial.print("\t");
    //    Serial.print("Hum: "); Serial.print(hum);
    Serial.println();
    node.clearResponseBuffer();
    node.clearTransmitBuffer();
  }

  delay(1000);
}