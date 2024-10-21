#include <Arduino.h>
#include <SPI.h>


// Pin untuk sensor soil moisture dan suhu
int soilMoisturePin = A0;  // A0 (untuk RK520-01 Moisture)
int soilTemperature = A1;  // A1 (untuk RK520-01 Temperature)

int sensorValue_1 = 0;
int sensorValue_2 = 0;



void setup() {
  // Initialize communication
  Serial.begin(9600);  // For monitoring
  
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

  

  delay(1000);
}
