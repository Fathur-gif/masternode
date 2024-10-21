#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

// Define pin mapping
#define ss 10         // NSS pin
#define rst 7         // RST pin
#define dio0 2        // DIO0 pin

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
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
  Serial.println("Mengirim pesan...");

  // Mulai mengirim pesan
  LoRa.beginPacket();
  LoRa.print("Hello World");
  LoRa.endPacket();

  // Jeda 5 detik sebelum mengirim pesan berikutnya
  delay(5000);
}
