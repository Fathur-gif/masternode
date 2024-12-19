#include <Arduino.h>

#include <EEPROM.h>

// Struktur konfigurasi
struct Config {
  int temperatureThreshold;
  float humidityThreshold;
};

Config config = {25, 75.5};

void setup() {
  Serial.begin(9600);

  // Simpan struktur ke EEPROM
  EEPROM.put(0, config);

  // Baca kembali struktur dari EEPROM
  Config loadedConfig;
  EEPROM.get(0, loadedConfig);

  // Tampilkan data yang dibaca
  Serial.print("Temperature Threshold: ");
  Serial.println(loadedConfig.temperatureThreshold);
  Serial.print("Humidity Threshold: ");
  Serial.println(loadedConfig.humidityThreshold);
}

void loop() {
  // Tidak ada proses di loop
}

