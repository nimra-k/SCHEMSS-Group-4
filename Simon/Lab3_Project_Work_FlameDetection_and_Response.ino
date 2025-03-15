// Using the Flame detection sensor and RGB LED for indication in relation with the
// cargo hold environment monitoring system(Project Work flow chart)//

int LED_R = D2;   // defines Red LED pin
int Buzzer = D3;  // defines Buzzer pin
int FlameDO = D0; // defines Flame sensor Digital input
int val;          // defines the digital variable from the sensor

void setup() {
  pinMode(LED_R, OUTPUT);   // defines Red LED pin as OUTPUT
  pinMode(Buzzer, OUTPUT);  // defines Buzzer pin as OUTPUT
  pinMode(FlameDO, INPUT);  // defines Flame sensor digital output pin as INPUT
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(FlameDO); // reads flame sensor digital INPUT value
  if (val == HIGH) {
    Serial.println("Flame Detected, action needed!!!"); // Print immediately
    digitalWrite(LED_R, HIGH);  // Turns on the Red LED
    digitalWrite(Buzzer, HIGH); // Turns on the Buzzer
    delay(500);                 // Delay for 0.5 seconds
    digitalWrite(Buzzer, LOW);  // Turns off the Buzzer
    delay(500);                 // Delay for 0.5 seconds
  } else {
    Serial.println("System okay!!!"); // Print status immediately
    delay(1000);               // Delay for 2 seconds
    digitalWrite(LED_R, LOW);  // Turns off the Red LED
    digitalWrite(Buzzer, LOW); // Turns off the Buzzer
    
  }
}
