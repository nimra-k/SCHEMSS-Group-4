// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin D4 as an output.
  pinMode(D4, OUTPUT);
  Serial.begin(230400);
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available() > 0) {
    // Read the incoming character
    char serialdata = Serial.read();

    Serial.print("Received: ");
    Serial.println(serialdata);
    
    if (serialdata == 'I') {
  digitalWrite(D4, LOW);  // turn the LED ON by letter I
  Serial.println("LED ON");
    }
    if (serialdata == 'M') {
  digitalWrite(D4, HIGH);   // turn the LED OFF by letter M
  Serial.println("LED OFF");
    }
  }
}
