#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into D1 on the ESP8266
#define ONE_WIRE_BUS D1

// A OneWire instance is created to communicate with the sensor
OneWire oneWire(ONE_WIRE_BUS);

// We connect the OneWire bus with the temperature sensor.
DallasTemperature sensors(&oneWire);

void setup(void) {
    Serial.begin(9600);
    Serial.println("Starting to measure temperature...");
    sensors.begin();
}

void loop(void) {
    delay(3000);

    sensors.requestTemperatures(); // Get temperature from sensor
    float temperature = sensors.getTempCByIndex(0);

    // Check if sensor failed to read data
    if (temperature == -127.00 || temperature < -55.0 || temperature > 125.0) {
        Serial.println("ERROR: Sensor reading invalid or out of range!");
        return;
    }

    Serial.print("Temperature is: ");
    Serial.print(temperature);
    Serial.print("°C");

    // Check if temperature exceeds 20°C
    if (temperature > 20.0) {
        Serial.println(" ⚠️ WARNING: Temperature exceeds 20°C!");
    } else {
        Serial.println(" ✅ Temperature is within safe limits.");
    }

    Serial.println("-----------------------------");
}
