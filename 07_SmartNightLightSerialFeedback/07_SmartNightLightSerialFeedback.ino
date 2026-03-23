/*
 * Author: Iwan
 * Project: 07_Smart_Night_Light_Serial_Feedback
 * Learning Objective: Implement dual-validation logic using Boolean variables 
 * and the 12-bit ADC of the ESP32.
 */

// --- Pin Definitions (Optimized for right-side wiring) ---
#define LdrPin 32         // Analog Input (ADC1_CH4)
#define PresencePin 33    // Digital Input (FC-51)
#define Led1 25           // Output Array
#define Led2 26           // Output Array
#define Led3 27           // Output Array
#define Led4 14           // Output Array
#define Led5 12           // Output Array

// --- Variable Definitions ---
int LdrValue = 0;
bool IsDark = false;
bool PresenceDetected = false;

void setup() {
  pinMode(LdrPin, INPUT);
  pinMode(PresencePin, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Led5, OUTPUT);
  
  // ESP32 Baud Rate for high-speed telemetry
  Serial.begin(115200);
  Serial.println("--- SYSTEM INITIALIZED: SINGLE-SIDE WIRING ---");
}

void loop() {
  // 1. Reading Analog Value (0-4095)
  LdrValue = analogRead(LdrPin);
  
  // 2. Presence Detection (Active LOW Logic conversion)
  PresenceDetected = !digitalRead(PresencePin); 

  // 3. Ambient Light Evaluation (Threshold for 12-bit ADC)
  if (LdrValue > 3000) { 
    IsDark = true;
  } else {
    IsDark = false;
  }

  // 4. Serial Feedback Telemetry
  Serial.print("LDR Value: ");
  Serial.print(LdrValue);
  Serial.print(" | IsDark: ");
  Serial.print(IsDark ? "TRUE" : "FALSE");
  Serial.print(" | Presence: ");
  Serial.println(PresenceDetected ? "TRUE" : "FALSE");

  // 5. Decision Logic (AND Operator)
  if (IsDark && PresenceDetected) {
    Serial.println(">> DECISION: LEDS ON");
    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, HIGH);
    digitalWrite(Led5, HIGH);
  } else {
    Serial.print(">> DECISION: LEDS OFF ");
    if (!IsDark) Serial.println("(Reason: Bright Environment)");
    else Serial.println("(Reason: No Presence Detected)");
    
    digitalWrite(Led1, LOW);
    digitalWrite(Led2, LOW);
    digitalWrite(Led3, LOW);
    digitalWrite(Led4, LOW);
    digitalWrite(Led5, LOW);
  }
  
  Serial.println("---------------------------------------");
  delay(500); 
}