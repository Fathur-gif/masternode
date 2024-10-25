#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define RX_PIN 19
#define TX_PIN 18
#define DE_RE_PIN 8  // Pin untuk mengontrol DE dan RE

void setup() {
  Serial.begin(9600);           // Untuk debugging
  Serial1.begin(9600);          // UART pada ATmega2560
  pinMode(DE_RE_PIN, OUTPUT);
}

void loop() {
  // Mode Kirim
  digitalWrite(DE_RE_PIN, HIGH);  // Aktifkan mode Transmitter
  Serial1.write("Requesting data...\n");  // Kirim perintah ke sensor
  delay(100);

  // Mode Terima
  digitalWrite(DE_RE_PIN, LOW);   // Aktifkan mode Receiver
  delay(100);

  // Baca data dari sensor
  while (Serial1.available()) {
    int data = Serial1.read();
    Serial.print("Data dari XY-MD02: ");
    Serial.println(data);
  }

  delay(1000);
}