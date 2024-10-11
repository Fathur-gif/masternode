#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin_1 = A0;  // A0
int soilMoisturePin_2 = A1;  // A1
int soilMoisturePin_3 = A2;  // A2 (untuk RK520-01 Moisture)
int soilTemperature = A3;    // A3 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;
int sensorValue_3 = 0;
int sensorValue_4 = 0;

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(115200);

  // Inisialisasi LoRa
  //LoRa.setPins(10, 9, 2);  // NSS, RST, DIO0
  //while (!LoRa.begin(920E6)) {
  //  Serial.println(".");
  //  delay(500);
  //}
  //LoRa.setSyncWord(0xF3);
  //Serial.println("LoRa siap!");
}

void loop() {
  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin_1);  // Sensor Moisture 1
  sensorValue_2 = analogRead(soilMoisturePin_2);  // Sensor Moisture 2
  sensorValue_3 = analogRead(soilMoisturePin_3);  // RK520-01 Moisture
  sensorValue_4 = analogRead(soilTemperature);    // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level 1: ");
  Serial.println(sensorValue_1);
  
  Serial.print("Soil Moisture Level 2: ");
  Serial.println(sensorValue_2);

  Serial.print("Soil Moisture Level 3: ");
  Serial.println(sensorValue_3);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_4);
  Serial.println("-------------------------------");

  // Buat string data untuk dikirimkan melalui LoRa
  //String data = String(sensorValue_1) + "," + 
  //              String(sensorValue_2) + "," + 
  //              String(sensorValue_3) + "," + 
  //              String(sensorValue_4);

  // Kirim data melalui LoRa
  //LoRa.beginPacket();
  //LoRa.print(data);
  //LoRa.endPacket();

  //Serial.println("Data dikirim: " + data);  

  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
