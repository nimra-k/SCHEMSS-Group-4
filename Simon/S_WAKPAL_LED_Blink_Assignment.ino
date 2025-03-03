// Dedicating M and D as On and Off
int On = HIGH;
int Off = LOW;

// Naming pin D4 as LED_pin
int LED_pin = D4;

void setup() {
  // Initializing digital Pin as output
  pinMode(LED_pin, OUTPUT);

  // Initializing Serial Monitor
  Serial.begin(9600);

  // Start with the LED turned off, becaue the ESP board's builtin LED is active off/LOW.
  digitalWrite(LED_pin, On);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming byte
    char command = Serial.read();

    // Print the received command
    Serial.print("Received command: ");
    Serial.println(command);

    // Switch LED on or off based on the command
    if (command == 'M') { // letting 'M' be the ON character
      digitalWrite(LED_pin, Off);
      Serial.println("Turning LED ON");
    } else if (command == 'D') { // letting 'D' be the OFF character
      digitalWrite(LED_pin, On);
      Serial.println("Turning LED OFF");
    }
  }
}
