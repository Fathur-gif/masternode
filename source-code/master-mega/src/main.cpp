#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
//#include <LoRa.h>

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;    // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

#define RS485_RX 19  // Pin untuk RX RS485
#define RS485_TX 18  // Pin untuk TX RS485

SoftwareSerial RS485Serial(RS485_RX, RS485_TX);

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(115200);
  RS485Serial.begin(9600);

  Serial.println("Inisialisasi sensor...");
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
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  sensorValue_2 = analogRead(soilTemperature);    // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_2);
  Serial.println("-------------------------------");

  if (RS485Serial.available()) {
    while (RS485Serial.available()) {
      char c = RS485Serial.read();
      Serial.write(c); // Kirim ke Serial Monitor
    }
  }

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
