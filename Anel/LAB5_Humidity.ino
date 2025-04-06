#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// === WiFi Configuration ===
const char* ssid = "iPhone";
const char* password = "zhanbolat";

// === MQTT Configuration ===
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "devices/NAPIoT-P2/anelieb12";

// === DHT Sensor Configuration ===
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi and MQTT Clients
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

// === Reconnect to MQTT if disconnected ===
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP-Humidity-Client")) {
      Serial.println("connected!");
    } else {
      Serial.print("❌ Failed, rc=");
      Serial.print(client.state());
      Serial.println(" — retrying in 5 seconds");
      delay(5000);
    }
  }
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// === Main Loop ===
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(3000); // Minimum delay between DHT11 reads

  float humidity = dht.readHumidity();

  // Validate reading
  if (isnan(humidity) || humidity < 20.0 || humidity > 90.0) {
    Serial.println("❌ Invalid humidity reading");
    return;
  }

  // Determine status
  const char* status;
  if (humidity <= 60.0) {
    status = "Safe";
  } else if (humidity > 60.0 && humidity <= 70.0) {
    status = "Caution";
  } else {
    status = "Warning";
  }

  // Log to Serial
  Serial.print("💧 Humidity: ");
  Serial.print(humidity);
  Serial.print(" % — Status: ");
  Serial.println(status);

  // Format JSON
  char payload[64];
  snprintf(payload, sizeof(payload),
           "{\"humidity\": %.1f, \"status\": \"%s\"}",
           humidity, status);

  // Publish to MQTT
  client.publish(mqtt_topic, payload);
  Serial.println("✅ Sent to MQTT");

  delay(2000); // Wait before next cycle
}
