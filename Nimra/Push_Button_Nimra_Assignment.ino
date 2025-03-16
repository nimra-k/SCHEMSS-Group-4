const int ledPin = D4;       // LED connected to D4
const int buttonPin = D1;    // Push button connected to D1
char serialInput;            // Variable to store serial input
bool ledState = LOW;         // LED state tracker

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor for button
    Serial.begin(9600);
}

void loop() {
    static bool lastButtonState = HIGH;  // Store last button state

    // Read button state
    bool buttonState = digitalRead(buttonPin);

    // Detect button press (change from HIGH to LOW)
    if (buttonState == LOW && lastButtonState == HIGH) {
        ledState = !ledState; // Toggle LED state
    }
    lastButtonState = buttonState; // Update button state

    // Read Serial Input
    if (Serial.available() > 0) {
        serialInput = Serial.read();

        if (serialInput == 'B') {
            ledState = LOW;  // Turn on LED via Serial
        } 
        else if (serialInput == 'I') {
            ledState = HIGH;   // Turn off LED via Serial
        }
    }

    // Apply LED state
    digitalWrite(ledPin, ledState);
}
