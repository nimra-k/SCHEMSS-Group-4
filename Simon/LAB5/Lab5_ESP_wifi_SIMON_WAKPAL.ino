#include <ESP8266WiFi.h>

// WiFi network parameters
const char* ssid     = "Simon's_GalaxyA525G";
const char* password = "123456789@TYty";

void setup() {
  // Start serial
  Serial.begin(115200);
  
  // Give Serial time to connect (especially important for some serial monitors)
  delay(5000);

  // Print connecting message
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
