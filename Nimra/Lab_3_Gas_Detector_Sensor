// -------------------------------------------------------------Digital Pin Code (Tested)--------------------------------------------------------------------------------------
//int MQ_PIN = D1;
//const int MQ_DELAY = 2000;

//void setup()
//{
  //Serial.begin(9600);
//}

//void loop()
//{
  //bool state= digitalRead(MQ_PIN);

  //if (!state)
  //{
    //Serial.println("Detection");
  //}
  //else
  //{
    //Serial.println("Not detected");
  //}
  //delay(MQ_DELAY);
//}

// -------------------------------------------------------------Analog Pin Code (Tested)--------------------------------------------------------------------------------------

//int sensorPin = A0;
//int threshold = 500;  // Adjust based on calibration

//void setup() {
  //Serial.begin(9600);
//}

//void loop() {
  //int sensorValue = analogRead(sensorPin);
  //VoltageValue = (5.0 / 1023.0)*sensorValue;
  //Serial.println(VoltageValue); 

  //if (sensorValue > threshold) {
    //Serial.println("Gas Detected!");
  //}
  //else 
  //Serial.println("No Gas Detected");

  //delay(1000);
//}

// -------------------------------------------------------------Analog Pin Code for Clean Air Resistance Value R0 (Tested)--------------------------------------------------------------------------------------
//const int sensorPin = A0;  // Analog pin connected to MQ-6
//const int RL = 5000;       // Load resistor value in ohms (5kΩ)

//void setup() {
  //Serial.begin(9600);
  //Serial.println("Calibrating MQ-6 Sensor");
//}

//void loop() {
  //float sensorValue = analogRead(sensorPin);  // Read raw ADC value (0-1023)
 // float sensorVoltage = sensorValue * (5.0 / 1023.0);  // Convert ADC to voltage
 // float Rs = ((5.0 - sensorVoltage) / sensorVoltage) * RL;  // Calculate Rs
  //float Ro = Rs / 10.0;  // Rs/Ro = 10 in clean air from Chart

  //Serial.print("Sensor Value: ");
 // Serial.print(sensorValue);
  //Serial.print("\t Rs: ");
 // Serial.print(Rs);
  //Serial.print(" Ω\t Ro: ");
 // Serial.print(Ro);
  //Serial.println(" Ω");

 // delay(2000);  // Wait 2 second before next reading

// -------------------------------------------------------------FINAL CODE--------------------------------------------------------------------------------------
const int sensorPin = A0;  // Analog pin connected to MQ-6
const int RL = 5000;       // Load resistor value in ohms (5kΩ)

void setup() {
  Serial.begin(9600);
  Serial.println("Calibrating MQ-6 sensor...");
}

void loop() {
  float sensorValue = analogRead(sensorPin);  // Read raw ADC value (0-1023)
  float sensorVoltage = sensorValue * (5.0 / 1023.0);  // Convert ADC to voltage
  float Rs = ((5.0 - sensorVoltage) / sensorVoltage) * RL;  // Calculate Rs
  float R0 = 35791.5; // Avg Value Calculated from Calibration
  float ratio = Rs/R0; 

  if (Rs/R0 < 0.9){
    Serial.print("Sensor Value of Rs/R0 : ");
    Serial.print(ratio);
    Serial.println("Gas Detected !:O"); //can be LPG or CH4 or alcohol
  }
  else
  Serial.print("Sensor Value of Rs/R0 : ");
  Serial.print(ratio);
  Serial.println("\nNo Gas Detected :D "); //can be LPG or CH4
    
  delay(1500);  // Wait 1.5 second before next reading
}
