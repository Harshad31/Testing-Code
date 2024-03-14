#include <Wire.h>  // Include the Wire library for I2C communication
#include <SHT4X.h> // Include the SHT4X library for SHT41 sensor

SHT4X sht41;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C bus
  sht41.begin(0x44); // Initialize SHT41 sensor
}

void loop() {
  // Read temperature and humidity from the SHT41 sensor
  float temperature = sht41.readTemperature();
  float humidity = sht41.readHumidity();

  // Check if reading is successful
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.println();
  } else {
    Serial.println("Failed to read from SHT41 sensor!");
  }

  delay(2000); // Delay between readings
}
