#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);

  // Inisialisasi LoRa
  LoRa.setPins(5, 14, 26);  // NSS, RST, DIO0
  if (!LoRa.begin(915E6)) {  // Sesuaikan frekuensi dengan transmitter (misalnya 433E6 atau 868E6)
    Serial.println("Gagal menginisialisasi LoRa");
    while (1);
  }
  Serial.println("LoRa receiver siap!");
}

void loop() {
  // Cek apakah ada paket yang diterima
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Baca data yang diterima
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    // Tampilkan data di Serial Monitor
    Serial.println("Data diterima: " + receivedData);
  }
}
