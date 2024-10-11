#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);

  // Inisialisasi LoRa
  LoRa.setPins(5, 14, 2);  // NSS, RST, DIO0
  if (!LoRa.begin(920E6)) {  // Sesuaikan frekuensi dengan transmitter (misalnya 433E6 atau 868E6)
    Serial.println("Gagal menginisialisasi LoRa");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
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
