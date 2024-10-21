#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

// Define pin mapping
#define ss 10         // NSS pin
#define rst 7         // RST pin
#define dio0 2        // DIO0 pin



void setup() {
  // Initialize communication
  Serial.begin(9600);  // For monitoring
  while (!Serial);

  Serial.println("LoRa Transmitter");

  // Inisialisasi LoRa dengan pin yang ditentukan
  LoRa.setPins(ss, rst, dio0);

  // Set frequency sesuai kebutuhan, 920 MHz untuk Indonesia
  if (!LoRa.begin(920E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }
  Serial.println("LoRa siap!");

}

void loop() {


  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  sensorValue_2 = analogRead(soilTemperature);  // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_2);

  // Mengirim data sensor melalui LoRa
  Serial.println("Mengirim data sensor melalui LoRa...");

  // Mulai mengirim paket LoRa
  LoRa.beginPacket();
  LoRa.print("Soil Moisture: ");
  LoRa.print(sensorValue_1);
  LoRa.print(", Soil Temperature: ");
  LoRa.print(sensorValue_2);
  LoRa.endPacket();

  delay(1000);
}
