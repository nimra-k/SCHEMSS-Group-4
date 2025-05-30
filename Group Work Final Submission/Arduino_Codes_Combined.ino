#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Livebox6-B35F";
const char* password = "4ntkh5hfdHPL";

// MQTT broker settings
const char* mqttServer = "broker.hivemq.com";//"test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

// MQTT topics
const char* mqttPubTopicFlame = "devices/Lab5sensor/ESP8266_wifi/flame_status";
const char* mqttSubTopic = "devices/Lab5sensor/ESP8266_wifi/control";
const char* mqttPubTopicTemp = "devices/NAPIoT-P2/anelieb/temperature";
const char* mqttPubTopicHumidity = "devices/NAPIoT-P2/anelieb/humidity";
const char* mqttPubTopicGas = "devices/NAPIoT-P2/anelieb/gas";

// Pins configuration
const int LED_RED = D0;
const int LED_BLUE = D5;
const int LED_GREEN = D6;
const int LED_YELLOW = D7;
const int FlameDO = D3;
const int gasSensorPin = A0;
const int BUZZER = D4;
const int FAN = D2;

// Gas sensor calibration
const int RL = 5000;
const float R0 = 35791.5;

// DHT11 sensor setup
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Manual override control
bool manualOverride = false;
bool manualState = false;

// Timing
unsigned long lastFlameMillis = 0;
unsigned long flameInterval = 1000;
unsigned long lastTempHumMillis = 0;
unsigned long tempHumInterval = 2000;
unsigned long lastGasMillis = 0;
unsigned long gasInterval = 3000;

// Fan timing
unsigned long fanOnUntil = 0;
const unsigned long fanDelayDuration = 3000;

void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  Serial.print("Received MQTT control: ");
  Serial.println(message);

  if (message == "1") {
    manualOverride = true;
    manualState = true;
    Serial.println("Manual ON: LEDs & Buzzer OFF");
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(FAN, LOW);
  } else if (message == "0") {
    manualOverride = true;
    manualState = false;
    Serial.println("Manual OFF: LEDs & Buzzer on");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(BUZZER, LOW);
    digitalWrite(FAN, HIGH);
  } else if (message == "auto") {
    manualOverride = false;
    Serial.println("Switched to Auto Mode");
  } else {
    Serial.println("Unknown command");
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("Simon_LED_001", mqttUser, mqttPassword)) {
      Serial.println("Connected!");
      client.subscribe(mqttSubTopic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqttSubTopic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(". Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(8000);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FlameDO, INPUT);
  pinMode(FAN, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(FAN, LOW);

  connectWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnectMQTT();
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long currentMillis = millis();

  // --- Flame Sensor ---
  if (currentMillis - lastFlameMillis >= flameInterval) {
    lastFlameMillis = currentMillis;
    int flameVal = digitalRead(FlameDO);

    if (!manualOverride) {
      if (flameVal == HIGH) {
        Serial.println("Flame Detected, action needed!!!");
        client.publish(mqttPubTopicFlame, "Flame Detected");
        digitalWrite(LED_RED, HIGH);
        digitalWrite(BUZZER, HIGH);
        digitalWrite(FAN, HIGH);
        fanOnUntil = millis() + fanDelayDuration;
      } else {
        Serial.println("No Flame Detected, system okay.");
        client.publish(mqttPubTopicFlame, "System okay");
        digitalWrite(LED_RED, LOW);
        digitalWrite(BUZZER, LOW);
        // Do NOT turn off FAN here
      }
    }
  }

  // --- Temperature & Humidity Sensor ---
  if (currentMillis - lastTempHumMillis >= tempHumInterval) {
    lastTempHumMillis = currentMillis;
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } else {
      const char* tempStatus;

      if (temperature <= 15.0) {
        tempStatus = "Safe";
        if (!manualOverride) digitalWrite(LED_GREEN, LOW);
      } else if (temperature > 10.0 && temperature <= 20.0) {
        tempStatus = "Caution";
        if (!manualOverride) digitalWrite(LED_GREEN, LOW);
      } else {
        tempStatus = "Warning";
        if (!manualOverride) {
          digitalWrite(LED_GREEN, HIGH);
          digitalWrite(BUZZER, HIGH);
          digitalWrite(FAN, HIGH);
          fanOnUntil = millis() + fanDelayDuration;
        }
      }

      if (!manualOverride) {
        if (humidity < 50.0 || humidity > 70.0) {
          digitalWrite(LED_YELLOW, HIGH);
          digitalWrite(BUZZER, HIGH);
          digitalWrite(FAN, HIGH);
          fanOnUntil = millis() + fanDelayDuration;
        } else {
          digitalWrite(LED_YELLOW, LOW);
          digitalWrite(BUZZER, LOW);
          // Do NOT turn off FAN here
        }
      }

      Serial.print("Temp: ");
      Serial.print(temperature);
      Serial.print(" °C — Status: ");
      Serial.println(tempStatus);

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      char tempPayload[64];
      snprintf(tempPayload, sizeof(tempPayload),
               "{\"temperature\": %.1f, \"status\": \"%s\"}",
               temperature, tempStatus);
      client.publish(mqttPubTopicTemp, tempPayload);

      char humPayload[64];
      snprintf(humPayload, sizeof(humPayload),
               "{\"humidity\": %.1f}",
               humidity);
      client.publish(mqttPubTopicHumidity, humPayload);
    }
  }

  // --- Gas Sensor ---
  if (currentMillis - lastGasMillis >= gasInterval) {
    lastGasMillis = currentMillis;
    float sensorValue = analogRead(gasSensorPin);
    float sensorVoltage = sensorValue * (5.0 / 1023.0);
    float Rs = ((5.0 - sensorVoltage) / sensorVoltage) * RL;
    float ratio = Rs / R0;

    Serial.print("Sensor Value of Rs/R0 : ");
    Serial.print(ratio);

    if (!manualOverride) {
      if (ratio < 0.5) {
        Serial.println(" Gas Detected !:O");
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(BUZZER, HIGH);
        digitalWrite(FAN, HIGH);
        fanOnUntil = millis() + fanDelayDuration;
        String gasPayloadDetected = "{\"gas\":\"detected\", \"ratio\": " + String(ratio, 2) + "}";
        client.publish(mqttPubTopicGas, gasPayloadDetected.c_str());
      } else {
        Serial.println(" No Gas Detected :D");
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(BUZZER, LOW);
        String gasPayloadNone = "{\"gas\":\"none\", \"ratio\": " + String(ratio, 2) + "}";
        client.publish(mqttPubTopicGas, gasPayloadNone.c_str());
        // Do NOT turn off FAN here
      }
    }
  }

  // --- Fan OFF Logic ---
  if (!manualOverride && millis() > fanOnUntil) {
    digitalWrite(FAN, LOW);
  }

  // Manual override OFF state logic
  if (manualOverride && manualState == false) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(FAN, LOW);
  }

  delay(50);
}
