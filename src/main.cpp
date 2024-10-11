#include "./Adafruit_MLX90393.h"
#include <Arduino.h>
#define HALL1_CS SS
#define HALL2_CS 12


Adafruit_MLX90393 hall_1 = Adafruit_MLX90393();
Adafruit_MLX90393 hall_2 = Adafruit_MLX90393();

SPIClass hall1SPI(FSPI);
SPIClass hall2SPI(0);
SPIClass FPForceSPI(HSPI);

void hallSetup(Adafruit_MLX90393& hall)
{
    hall.setGain(MLX90393_GAIN_1X);
    hall.setResolution(MLX90393_Z, MLX90393_RES_16);
    hall.setOversampling(MLX90393_OSR_0);
    hall.setFilter(MLX90393_FILTER_1);
    hall.startBurstMode(MLX90393_Z);
}

void setup()
{
    Serial.begin(115200);
    delay(3000);
    FPForceSPI.begin();
    Serial.println(FPForceSPI.pinSS());

    if (!hall_1.begin_SPI(HALL1_CS, &hall1SPI)) {
        Serial.println("No hall_1 found ... check your wiring?");
        while (1) {
            delay(10);
        }
    }
    Serial.println("Found a MLX90393 hall_1");
    hallSetup(hall_1);
}

void loop()
{
    float x, y, z;
    uint8_t num_readings = 1;

    // Non-blocking read with millis() for fast loop execution
    static unsigned long lastReadTime = 0;
    unsigned long interval = 1; // Read every 10ms (adjustable)

    unsigned long currentMillis = millis();

    if (currentMillis - lastReadTime >= interval) {
        if (hall_1.readBurstData(&x, &y, &z, num_readings)) {
            // Serial.print("Z axis value: ");
            // Serial.println(z, 6);
        } else {
            Serial.println("Error reading burst data");
        }
        lastReadTime = currentMillis;
    }
}
