
//Using the Flame detection sensor and RGB LED for indication for Lab3_Exercise_Combined//
int LED_R = D2;   // defines RGB red LED pin
int LED_G = D3;   // defines RGB green LED pin
int FlameDO = D0;   // deifne Flame sensor Digital Output
int val;   // defines the digital variable from the sensor

void setup() {
  
pinMode(LED_R, OUTPUT); // defines Red LED pin as OUTPUT
pinMode(LED_G, OUTPUT); // defines Green LED pin as OUTPUT
pinMode(FlameDO,INPUT); // defines Flame sensor digital output pin as INPUT
Serial.begin(9600);

}

void loop() {
  
  val = digitalRead(FlameDO); // reads flame sensor digital INPUT value
  if (val==HIGH)
  {
    
    digitalWrite(LED_R, HIGH); // puts on the red LED
    digitalWrite(LED_G, LOW); // Puts off the Green LED
    Serial.println("Flame Detected, action nedde!!!");
    delay(500);
  }
  else 
  {
    digitalWrite(LED_R, LOW); //Puts off the Red LED
    digitalWrite(LED_G, HIGH); //Puts on the Green LED
    Serial.println("System okay!!!");
    delay(500);
  }
}
