#include <EEPROM.h>
#include <Arduino.h>
#include <SPI.h>



// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

// Threshold untuk soil moisture dan suhu
int moistureThreshold;
int temperatureThreshold;


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
  Serial.begin(9600);
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
  Serial.println("ini code baru");


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
