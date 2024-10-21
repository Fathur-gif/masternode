#include <arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Define pin mapping for LoRa with ESP32
#define ss 5         // NSS pin
#define rst 14       // RST pin
#define dio0 26      // DIO0 pin

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver (ESP32)");

  // Inisialisasi LoRa
  LoRa.setPins(ss, rst, dio0);  // Atur pin NSS, RST, dan DIO0

  if (!LoRa.begin(920E6)) {  // Sesuaikan frekuensi
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }
  Serial.println("LoRa siap menerima pesan!");
}

void loop() {
  // Cek apakah ada data yang diterima
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Jika ada data, tampilkan di Serial Monitor
    Serial.print("Pesan diterima: ");
    while (LoRa.available()) {
      String message = LoRa.readString();
      Serial.print(message);
    }
    Serial.println();
  }
}
