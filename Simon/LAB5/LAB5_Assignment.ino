#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Simon's_GalaxyA525G";
const char* password = "123456789@TYty";

// MQTT broker settings
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";         
const char* mqttPassword = "";     

WiFiClient espClient;
PubSubClient client(espClient);

// Pins
int LED_R = D4;
int Buzzer = D3;
int FlameDO = D0;
int val;

// MQTT topics
const char* mqttPubTopic = "devices/Lab5sensor/ESP8266_wifi/flame_status";
const char* mqttSubTopic = "devices/Lab5sensor/ESP8266_wifi/control";

// Manual override control
bool manualOverride = false;
bool manualState = false;

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  message.trim();  // Remove whitespace or newline

  Serial.print("Received MQTT control: ");
  Serial.println(message);

  if (message == "1") {
    digitalWrite(LED_R, HIGH);
    digitalWrite(Buzzer, HIGH);
    manualOverride = true;
    manualState = true;
    Serial.println("Manual ON: LED & Buzzer");
  } else if (message == "0") {
    digitalWrite(LED_R, LOW);
    digitalWrite(Buzzer, LOW);
    manualOverride = true;
    manualState = false;
    Serial.println("Manual OFF: LED & Buzzer");
  } else if (message == "auto") {
    manualOverride = false;
    Serial.println("Switched to Auto Mode");
  } else {
    Serial.println("Unknown command");
  }
}

void setup() {
  pinMode(LED_R, OUTPUT);   
  pinMode(Buzzer, OUTPUT);  
  pinMode(FlameDO, INPUT);  

  Serial.begin(115200);
  delay(8000);  // Give time for Serial Monitor to open

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("devices/Lab5sensor/ESP8266_wifi/test", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT!");
      client.subscribe(mqttSubTopic);
    } else {
      Serial.print("MQTT connect failed: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  client.loop();

  val = digitalRead(FlameDO);

  // Always publish sensor status
  if (val == HIGH) {
    Serial.println("Flame Detected, action needed!!!");
    client.publish(mqttPubTopic, "Flame Detected");
  } else {
    Serial.println("System okay!!!");
    client.publish(mqttPubTopic, "System okay");
  }

  // Only control LED & Buzzer in auto mode
  if (!manualOverride) {
    if (val == LOW) {
      digitalWrite(LED_R, HIGH);
      digitalWrite(Buzzer, HIGH);
      delay(500);
      digitalWrite(Buzzer, LOW);
      delay(500);
    } else {
      digitalWrite(LED_R, LOW);
      digitalWrite(Buzzer, LOW);
      delay(2000);
    }
  } else {
    // Manual mode: maintain state
    digitalWrite(LED_R, manualState ? HIGH : LOW);
    digitalWrite(Buzzer, manualState ? HIGH : LOW);
    delay(2000); // slower loop to match status update frequency
  }
}
