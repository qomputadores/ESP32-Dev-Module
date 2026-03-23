/*
 * Author: Iwan
 * Project: 08_Smart_Trash_Can_Serial_Monitoring
 * Learning Objective: Implement mechatronic control using the ESP32Servo library, 
 * including safety blocking loops and smooth closing algorithms.
 */

#include <ESP32Servo.h>

// --- Pin Definitions (Right side of 30-pin ESP32) ---
#define ServoPin 18       // PWM Output for Servo
#define ObstacleSensor 19  // Digital Input (FC-51)
#define RedLed 12          // Status: Operation/Open
#define GreenLed 13        // Status: Standby/Ready

// --- Objects and Variables ---
Servo TrashServo;
bool IsHandPresent = false;
bool IsLidOpen = false;

void setup() {
  // Attaching the servo to the ESP32 PWM timer
  TrashServo.attach(ServoPin);
  pinMode(ObstacleSensor, INPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  
  // Initial State: System Ready / Lid Closed (0 degrees)
  TrashServo.write(0);
  digitalWrite(GreenLed, HIGH);
  digitalWrite(RedLed, LOW);
  IsLidOpen = false;
  
  // High-speed telemetry for ESP32
  Serial.begin(115200);
  Serial.println("--- SYSTEM START: SMART TRASH CAN ---");
  Serial.println("STATUS: STANDBY - LID CLOSED");
}

void loop() {
  // 1. Sensor Detection (Active LOW Logic conversion using '!')
  IsHandPresent = !digitalRead(ObstacleSensor);

  // 2. Trigger: Lid opens only if hand is detected AND lid is currently closed
  if (IsHandPresent && !IsLidOpen) {
    Serial.println(">> EVENT: OBSTACLE DETECTED");
    Serial.println(">> ACTION: OPENING LID (FAST)");
  
    // Quick Opening Sequence for better UX
    TrashServo.write(90);
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    IsLidOpen = true;
    Serial.println("STATUS: LID OPENED");
    
    // 3. SAFETY BLOCK: Hold lid open while hand is still detected
    while(!digitalRead(ObstacleSensor)) {
      Serial.println("STATUS: WAITING (HAND STILL PRESENT)");
      delay(500);
    }
  
    // 4. USER NOTICE: Safety Countdown before closing
    Serial.println(">> EVENT: HAND REMOVED");
    for (int i = 5; i > 0; i--) { // Reduced to 5s for laboratory efficiency
      Serial.print(">> NOTICE: CLOSING IN ");
      Serial.print(i);
      Serial.println(" SECONDS...");
      delay(1000);
    }

    // 5. MECHATRONIC CARE: Controlled Slow Closing Sequence
    Serial.println(">> ACTION: CLOSING LID (SLOW)");
    for (int angle = 90; angle >= 0; angle--) {
      TrashServo.write(angle);
      delay(30); // Controls the closing speed
    }
    
    // Reset to Standby
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, HIGH);
    IsLidOpen = false;
    Serial.println("STATUS: STANDBY - LID CLOSED");
    Serial.println("---------------------------------------");
  }

  delay(100);
}