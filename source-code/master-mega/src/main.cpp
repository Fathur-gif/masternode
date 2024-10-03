#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

int soilMoisturePin_1 = A0;  
int sensorValue_1 = 0;
int soilMoisturePin_2 = A1;
int sensorValue_2 = 0;
int soilMoisturePin_3 = A2;
int sensorValue_3 = 0;
int soilTemperature = A3;
int sensorValue_4 = 0;


void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(9600);

  LoRa.setPins(10, 9, 2);  // NSS, RST, DIO0
  if (!LoRa.begin(920E6)) {  // Sesuaikan frekuensi 920 MHz atau sesuai kebutuhan
    Serial.println("Gagal menginisialisasi LoRa");
    while (1);
  }
  Serial.println("LoRa siap!");
}

void loop() {
  // Membaca nilai analog dari sensor kelembaban tanah
  sensorValue_1 = analogRead(soilMoisturePin_1);
  sensorValue_2 = analogRead(soilMoisturePin_2);
  sensorValue_3 = analogRead(soilMoisturePin_3);
  sensorValue_4 = analogRead(soilTemperature);

  // Mengubah nilai ke rentang yang lebih sesuai, misalnya 0-100 persen
  //int moisturePercent = map(sensorValue_1, 0, 4095, 0, 100);
  //int moisturePercent_2 = map(sensorValue_2, 0, 4095,0,100);

  // Tampilkan nilai kelembaban tanah ke serial monitor
  Serial.print("Soil Moisture Level 1: ");
  Serial.println(sensorValue_1);
  //Serial.println("%");
  Serial.print("Soil Moisture Level 2: ");
  Serial.println(sensorValue_2);
  //Serial.println("%");
  Serial.print("Soil Moisture Level 3: ");
  Serial.println(sensorValue_3);
  //Serial.println("%");
  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_4);
  Serial.println("-------------------------------");

  // Buat string data untuk dikirimkan melalui LoRa
  String data = String(sensorValue_1) + "," + 
                String(sensorValue_2) + "," + 
                String(sensorValue_3) + "," + 
                String(sensorValue_4);

  // Kirim data melalui LoRa
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();

  Serial.println("Data dikirim: " + data);  
  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
