#include <Arduino.h>
#include <SPI.h>
#include <ModbusMaster.h>
#include <LoRa.h>

#define ss 10         // NSS pin
#define rst 7         // RST pin
#define dio0 2        // DIO0 pin
#define RX1 19
#define TX1 18 // RX, TX
#define MAX485_DE 33
#define MAX485_RE_NEG 31

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
// int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

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

void setup() {
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
  Serial.println("LoRa Transmitter");
  
  LoRa.setPins(ss, rst, dio0);

  // Set frequency sesuai kebutuhan, 920 MHz untuk Indonesia
  if (!LoRa.begin(920E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }
  Serial.println("LoRa siap!");

  Serial1.begin(9600);
  while (!Serial1) {
  Serial.println("loop for init software serial");
  }
  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {

  uint8_t result;
  uint16_t data[2];
  // Serial.println("get data");
  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  // sensorValue_2 = analogRead(soilTemperature);  // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  // Serial.print("Soil Temperature: ");
  // Serial.println(sensorValue_2);

  result = node.readInputRegisters(1, 2);
  if (result == node.ku8MBSuccess)
  {
    // Serial.print("Temp: ");
    // Serial.println(node.getResponseBuffer(0)/10.0f);
    // Serial.print("Humi: ");
    // Serial.println(node.getResponseBuffer(1)/10.0f);
    // Serial.println();
  }
  
  // Serial.println("Mengirim data sensor melalui LoRa...");

  // Mulai mengirim paket LoRa
  LoRa.beginPacket();
  // LoRa.print("Soil Moisture: ");
  LoRa.print(sensorValue_1);
  // LoRa.print(", Soil Temperature: ");
  // LoRa.print(sensorValue_2);
  // LoRa.print(", Temp:  ");
  LoRa.print(node.getResponseBuffer(0)/10.0f);
  // LoRa.print(", Humi: ");
  LoRa.print(node.getResponseBuffer(1)/10.0f);
  LoRa.endPacket();

  delay(2000);
}
