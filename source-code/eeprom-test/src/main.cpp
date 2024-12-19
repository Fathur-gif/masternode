#include <Arduino.h>

#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  
  // Simpan nilai pada address 0
  int configValue = 42; // Nilai konfigurasi
  EEPROM.write(0, configValue);
  
  // Baca nilai dari EEPROM
  int readValue = EEPROM.read(0);
  Serial.print("Nilai yang dibaca dari EEPROM: ");
  Serial.println(readValue);
}

void loop() {
  // Tidak ada proses di loop
}
