#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// === CONFIGURATION ===
// WiFi credentials
const char* ssid = "iPhone";
const char* password = "zhanbolat";

// MQTT broker
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "devices/NAPIoT-P2/anelieb";

// DHT11 sensor setup
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// === Connect to WiFi ===
void setup_wifi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// === Reconnect to MQTT broker if disconnected ===
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP-DHT11-Client")) {
      Serial.println("connected!");
    } else {
      Serial.print("❌ Failed, rc=");
      Serial.print(client.state());
      Serial.println(" — retrying in 5 seconds");
      delay(5000);
    }
  }
}

// === SETUP ===
void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// === LOOP ===
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(3000); // Wait before reading again

  float temperature = dht.readTemperature();

  if (isnan(temperature) || temperature < 0.0 || temperature > 50.0) {
    Serial.println("❌ Invalid temperature reading");
    return;
  }

  // Determine status
  const char* status;
  if (temperature <= 15.0) {
    status = "Safe";
  } else if (temperature > 15.0 && temperature <= 20.0) {
    status = "Caution";
  } else {
    status = "Warning";
  }

  // Print to Serial
  Serial.print("🌡️ Temp: ");
  Serial.print(temperature);
  Serial.print(" °C — Status: ");
  Serial.println(status);

  // Format JSON
  char payload[64];
  snprintf(payload, sizeof(payload),
           "{\"temperature\": %.1f, \"status\": \"%s\"}",
           temperature, status);

  // Publish
  client.publish(mqtt_topic, payload);
  Serial.println("✅ Sent to MQTT");

  delay(2000); // Wait before next cycle
}
