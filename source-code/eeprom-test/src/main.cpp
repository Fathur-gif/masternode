#include <EEPROM.h>
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
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

// Threshold untuk soil moisture dan suhu
int moistureThreshold;
int temperatureThreshold;

ModbusMaster node;
void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}
void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

// Fungsi untuk menyimpan integer (2 byte) ke EEPROM
void EEPROMWriteInt(int address, int value) {
  byte highByte = value >> 8; // Ambil 8 bit teratas
  byte lowByte = value & 0xFF; // Ambil 8 bit terbawah
  EEPROM.write(address, highByte);
  EEPROM.write(address + 1, lowByte);
}

// Fungsi untuk membaca integer (2 byte) dari EEPROM
int EEPROMReadInt(int address) {
  byte highByte = EEPROM.read(address);
  byte lowByte = EEPROM.read(address + 1);
  return (highByte << 8) | lowByte; // Gabungkan menjadi integer
}

void setup() {
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);
  while (!Serial); // Tunggu hingga Serial Monitor siap

  Serial.println("LoRa Transmitter");
  Serial.println("Ketik 'read [alamat]' untuk membaca data dari EEPROM");
  Serial.println("Contoh: read 0");

  LoRa.setPins(ss, rst, dio0);

  // Set frequency sesuai kebutuhan, 920 MHz untuk Indonesia
  if (!LoRa.begin(920E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }
  Serial.println("LoRa siap!");

  Serial1.begin(9600);
  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Baca threshold dari EEPROM
  moistureThreshold = EEPROMReadInt(0); // Lokasi 0 untuk moisture
  temperatureThreshold = EEPROMReadInt(2); // Lokasi 2 untuk temperature

  // Default nilai threshold jika EEPROM kosong
  if (moistureThreshold == 0xFFFF) moistureThreshold = 50; // Jika nilai tidak valid
  if (temperatureThreshold == 0xFFFF) temperatureThreshold = 25;

  Serial.print("Moisture Threshold (EEPROM): ");
  Serial.println(moistureThreshold);
  Serial.print("Temperature Threshold (EEPROM): ");
  Serial.println(temperatureThreshold);
}

void loop() {
  uint8_t result;
  uint16_t data[2];

  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  sensorValue_2 = analogRead(soilTemperature);  // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_2);

  // Baca data Modbus (contoh sensor tambahan)
  result = node.readInputRegisters(1, 2);
  if (result == node.ku8MBSuccess) {
    Serial.print("Temp: ");
    Serial.println(node.getResponseBuffer(0) / 10.0f);
    Serial.print("Humi: ");
    Serial.println(node.getResponseBuffer(1) / 10.0f);
    Serial.println();
  }

  // Kirim data melalui LoRa
  // LoRa.beginPacket();
  // LoRa.print("Soil Moisture: ");
  // LoRa.print(sensorValue_1);
  // LoRa.print(", Soil Temperature: ");
  // LoRa.print(sensorValue_2);
  // LoRa.print(", Temp: ");
  // LoRa.print(node.getResponseBuffer(0) / 10.0f);
  // LoRa.print(", Humi: ");
  // LoRa.print(node.getResponseBuffer(1) / 10.0f);
  // LoRa.endPacket();

  // Periksa perintah dari Serial Monitor
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Baca input hingga newline
    input.trim(); // Hapus karakter spasi atau newline berlebih

    // Periksa apakah perintah adalah "read"
    if (input.startsWith("read")) {
      int address = input.substring(5).toInt(); // Ambil alamat setelah "read "
      if (address >= 0 && address < EEPROM.length() - 1) {
        // Baca data dari EEPROM
        int value = EEPROMReadInt(address);
        Serial.print("Data di EEPROM alamat ");
        Serial.print(address);
        Serial.print(": ");
        Serial.println(value);
        Serial.println("");
      } else {
        Serial.println("Alamat EEPROM tidak valid!");
      }
    } else if (input == "m") { // Update moisture threshold
      EEPROMWriteInt(0, sensorValue_1);
      Serial.println("Moisture threshold updated!");
      Serial.println("");
    } else if (input == "t") { // Update temperature threshold
      EEPROMWriteInt(2, sensorValue_2);
      Serial.println("Temperature threshold updated!");
      Serial.println("");
    } else {
      Serial.println("Perintah tidak dikenali!");
    }
  }

  delay(2000); // Delay untuk loop
}
