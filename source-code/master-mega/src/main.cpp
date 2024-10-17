#include <Arduino.h>
#include <SPI.h>

// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;

// RS485 Communication setup
const int RS485_DIR = 8;  // Pin for RS485 DE/RE control

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(9600);
  Serial1.begin(9600); // RS485 communication on pins 19 (RX) and 18 (TX)

  // Set RS485 direction pin
  pinMode(RS485_DIR, OUTPUT);
  digitalWrite(RS485_DIR, LOW);  // Receiver mode
  
  Serial.println("Starting...");
}

void loop() {
  // Membaca nilai analog dari sensor kelembaban tanah dan suhu
  sensorValue_1 = analogRead(soilMoisturePin);  // RK520-01 Moisture
  sensorValue_2 = analogRead(soilTemperature);  // RK520-01 Temperature

  // Tampilkan nilai kelembaban tanah dan suhu ke serial monitor
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue_1);

  Serial.print("Soil Temperature: ");
  Serial.println(sensorValue_2);

  // Reading data from XY-MD02 over RS485
  String xy_md02_data = readXYMD02();
  Serial.print("XY-MD02 Data: ");
  Serial.println(xy_md02_data);
  
  Serial.println("-------------------------------");

  delay(1000);
}

String readXYMD02() {
  // Send request to XY-MD02
  digitalWrite(RS485_DIR, HIGH);  // Enable RS485 write mode
  Serial1.write(0x01);  // Modify with XY-MD02 specific command
  Serial1.write(0x03);  // Example function code for reading registers
  Serial1.write(0x00);  // Register address high byte
  Serial1.write(0x00);  // Register address low byte
  Serial1.write(0x00);  // Register count high byte
  Serial1.write(0x01);  // Register count low byte
  Serial1.write(0x84);  // CRC high byte (adjust as needed)
  Serial1.write(0x0A);  // CRC low byte (adjust as needed)
  digitalWrite(RS485_DIR, LOW);  // Set RS485 to read mode

  // Read response
  delay(100);  // Wait for sensor response
  String response = "";
  while (Serial1.available()) {
    response += char(Serial1.read());
  }
  
  return response;
}
