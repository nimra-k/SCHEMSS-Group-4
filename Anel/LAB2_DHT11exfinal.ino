#include "DHT.h"

#define DHTTYPE DHT11
#define dht_dpin D1

DHT dht(dht_dpin, DHTTYPE);

void setup() {
    dht.begin();
    Serial.begin(9600);
    delay(10);
    Serial.println("Initializing ESP8266...");
}

void loop() {
    delay(3000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // ✅ Check if sensor failed to read data
    if (isnan(h) || isnan(t) || t < 0.0 || t > 50.0 || h < 20.0 || h > 90.0) {
        Serial.println("ERROR: Invalid or out-of-range DHT11 sensor reading!");
        return; // Stop execution and avoid printing incorrect values
    }

    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("% ");

    Serial.print("temperature = ");
    Serial.print(t);
    Serial.println("C");

    // ✅ Check if temperature exceeds 20°C
    if (t > 20.0) {
        Serial.println(" ⚠️ WARNING: Temperature exceeds 20°C!");
    } else {
        Serial.println(" ✅ Temperature is within safe limits.");
    }

    // ✅ Check if humidity exceeds 70%
    if (h > 70.0) {
        Serial.println(" ⚠️ WARNING: Humidity exceeds 70%!");
    } else {
        Serial.println(" ✅ Humidity is within safe limits.");
    }

    Serial.println("-----------------------------");
}
