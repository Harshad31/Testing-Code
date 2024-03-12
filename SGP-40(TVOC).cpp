#include <Wire.h>   // Include the Wire library for I2C communication
#include <SGP40.h>  // Include the SGP40 library for SGP40 sensor

// Define the I2C address of your SGP40 sensor
#define SGP40_I2C_ADDRESS 0x58 // Example address, change according to your sensor's address

SGP40 sgp40; // Initialize SGP40 sensor with specified I2C address

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C bus
  sgp40.begin(); // Initialize SGP40 sensor
}

void loop() {
  // Read VOC index from the SGP40 sensor
  uint16_t VOC_index = sgp40.getVOCIndex();

  // Check if reading is successful
  if (VOC_index != SGP40_ERR_INVALID_VALUE) {
    Serial.print("VOC Index: ");
    Serial.println(VOC_index);

    Serial.println();
  } else {
    Serial.println("Failed to read from SGP40 sensor!");
  }

  delay(2000); // Delay between readings
}
