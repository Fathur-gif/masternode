#include <Arduino.h>
#include <SPI.h>
#include <ModbusMaster.h>

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

// RS485 Communication setup
const int RS485_DIR = 8;  // Pin for RS485 DE/RE control

void setup() {
  // Initialize communication
  Serial.begin(9600);  // For monitoring
  Serial1.begin(9600); // RS485

  // RS485 direction control
  pinMode(RS485_DIR, OUTPUT);
  digitalWrite(RS485_DIR, LOW);  // Start in read mode

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
  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  sensorValue_2 = analogRead(soilTemperature);  // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_2);

  // Reading data from XY-MD02 over RS485
  if (result == node.ku8MBSuccess) {
    data = node.getResponseBuffer(0x00);  // Get the first register response
    Serial.print("XY-MD02 Data: ");
    Serial.println(data);
  } else {
    Serial.print("Modbus Error: ");
    Serial.println(result, HEX);
  }

  delay(1000);
}
