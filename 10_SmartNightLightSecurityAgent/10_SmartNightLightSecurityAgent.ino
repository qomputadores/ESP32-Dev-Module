/*
 * Author: Iwan
 * Project: 10_Smart_Night_Light_Security_Agent
 * Learning Objective: Implement advanced flow control using Switch Case 
 * structures, modular logic, and state-based command validation.
 *
 * This material is for classroom use only. 
 * Commercial distribution is strictly prohibited.
 *
 */

#include <Adafruit_NeoPixel.h>

// --- PREPROCESSOR MACROS ---
// Converts the object identifier into a literal string for serial feedback.
#define GetName(Variable) #Variable

// --- Hardware Mapping ---
#define LdrPin 32       // Analog Input for light intensity (0-4095).
#define PresencePin 33  // Digital Input for the FC-51 presence sensor.
#define NeoPixelPin 13  // PWM output for the WS2812B LED array.
#define BuzzerPin 15    // Digital output for the Active Buzzer.
#define NumPixels 12    // Constant for the total number of LEDs.

// --- STUDENT PERSONALIZATION AREA ---
// INSTRUCTION: Replace 'LastNameFirstName_10C' with actual personal data.
// This identifier must be consistent across all function calls.
Adafruit_NeoPixel LastNameFirstName_10C(NumPixels, NeoPixelPin, NEO_GRB + NEO_KHZ800);

// --- Global Control Variables ---
int LdrValue = 0;
int IntrudersCount = 0;
char CurrentMode = 'L';  // Operational state: 'L' = Lamp / 'S' = Security.
bool IsWarmTone = true;  // Visual palette spectrum state.
bool PresenceDetected = false;

void setup() {
  // --- TELEMETRY INITIALIZATION ---
  Serial.begin(115200);

  // --- PIN CONFIGURATION ---
  pinMode(LdrPin, INPUT);
  pinMode(PresencePin, INPUT);
  pinMode(BuzzerPin, OUTPUT);

  // --- NEOPIXEL INITIALIZATION ---
  // Calling function to initialize hardware communication.
  LastNameFirstName_10C.begin();
  LastNameFirstName_10C.setBrightness(100);  // Intensity set to a 8-bit value (0-255).
  LastNameFirstName_10C.show();

  // --- STARTUP FEEDBACK ---
  Serial.println("--- SYSTEM INITIALIZED: SWITCH CASE LOGIC ---");
  Serial.print("Project Owner Identifier: ");
  // Calling the macro
  Serial.println(GetName(LastNameFirstName_10C));
  Serial.println("--------------------------------------------");
  Serial.println("");
  Serial.println("Commands: 'L' (Lamp), 'S' (Security), 'W' (Warm), 'C' (Cold)");
  Serial.println("");
}

void loop() {
  // --- 1. SENSOR DATA ACQUISITION ---
  LdrValue = analogRead(LdrPin);
  // Presence sensor uses active LOW logic (0 when detecting); '!' inverts it.
  PresenceDetected = !digitalRead(PresencePin);

  // --- 2. SERIAL INTERFACE HANDLING ---
  if (Serial.available() > 0) {
    char IncomingByte = Serial.read();
    // Calling function to handle serial command logic.
    HandleSystemModes(IncomingByte);
  }

  // --- 3. EXECUTION SELECTOR (STATE MACHINE) ---
  // The 'switch' command jumps directly to the active operational mode.
  switch (CurrentMode) {
    case 'L':
      // Calling function to execute smart lamp logic.
      ExecuteLampMode();
      break;

    case 'S':
      // Calling function to execute security agent logic.
      ExecuteSecurityMode();
      break;

    default:
      // Calling hardware methods to clear the state in case of error.
      LastNameFirstName_10C.clear();
      LastNameFirstName_10C.show();
      break;
  }

  delay(50);
}

// --- MODULAR LOGIC FUNCTIONS ---

/* 
 * Function: HandleSystemModes
 * Description: Interprets serial input and validates commands based on active state.
 */
void HandleSystemModes(char Command) {
  switch (Command) {
    case 'S':
    case 's':
      CurrentMode = 'S';
      IntrudersCount = 0;
      // Calling function to provide visual and acoustic mode-change feedback.
      NotifySystemState(255, 165, 0);
      Serial.println(">> MODE: SECURITY AGENT ARMED");
      break;

    case 'L':
    case 'l':
      CurrentMode = 'L';
      // Calling function to provide visual and acoustic mode-change feedback.
      NotifySystemState(0, 0, 255);
      Serial.print(">> MODE: SMART LAMP ACTIVE. Intrusions detected: ");
      Serial.println(IntrudersCount);
      break;

    case 'W':
    case 'w':
      // State Validation: Tone change permitted only in Lamp Mode.
      if (CurrentMode == 'L') {
        IsWarmTone = true;
        // Calling function to animate the transition to warm tone.
        RunSequentialTransition(255, 100, 20);
        Serial.println(">> TONE: WARM SPECTRUM SELECTED");
      } else {
        Serial.println(">> ACCESS DENIED: Cannot change tone in Security Mode.");
        tone(BuzzerPin, 300, 200);
      }
      break;

    case 'C':
    case 'c':
      // State Validation: Tone change permitted only in Lamp Mode.
      if (CurrentMode == 'L') {
        IsWarmTone = false;
        // Calling function to animate the transition to cold tone.
        RunSequentialTransition(200, 200, 255);
        Serial.println(">> TONE: COLD SPECTRUM SELECTED");
      } else {
        Serial.println(">> ACCESS DENIED: Cannot change tone in Security Mode.");
        tone(BuzzerPin, 300, 200);
      }
      break;

    default:
      /* 
       * CONDITION: Only process as an error if the character is NOT a New Line ('\n') 
       * or a Carriage Return ('\r'). These characters are often sent by the Serial Monitor 
       * but are not intended to be commands.
       */
      if (Command != '\n' && Command != '\r') {
        Serial.println(">> ERROR: Protocol command not recognized.");
        tone(BuzzerPin, 400, 200);
      }
      break;
  }
}

/* 
 * Function: ExecuteLampMode
 * Description: Automates lighting based on LDR levels with a fade-in effect.
 */
void ExecuteLampMode() {
  if (LdrValue > 3000) {
    /* 
     * ITERATIVE BLOCK: Executed when LdrValue exceeds the threshold (3000).
     * This 'for' loop creates a progressive 'Dimming' effect to increase 
     * brightness smoothly from 0 to 150.
     */
    for (int Brightness = 0; Brightness <= 150; Brightness += 5) {
      // CONDITION: Check which color spectrum is currently active.
      if (IsWarmTone) {
        /* 
         * Calling the 'fill' method to set a Warm Tone.
         * Math Logic: Red is at full 'Brightness', Green is halved, and Blue is 10%.
         * This ratio creates a sunset/amber color.
         */
        LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(Brightness, Brightness / 2, Brightness / 10));
      } else {
        /* 
         * Calling the 'fill' method to set a Cold Tone.
         * Math Logic: Red, Green, and Blue are equal, resulting in a White/Cold light.
         */
        LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(Brightness, Brightness, Brightness));
      }
      // Calling hardware method to update LED state.
      LastNameFirstName_10C.show();
      delay(10);
    }
  } else {
    // Calling hardware methods to shut down the lighting.
    LastNameFirstName_10C.clear();
    LastNameFirstName_10C.show();
  }
}

/* 
 * Function: ExecuteSecurityMode
 * Description: Manages the emergency response sequences and increments the intrusion database.
 */
void ExecuteSecurityMode() {

  // CONDITION: Check if the infrared sensor has detected movement (Boolean state).
  if (PresenceDetected) {

    // Incrementing the global counter to record the number of detected intrusions.
    IntrudersCount++;

    // Calling Serial method to send an urgent telemetry alert to the monitor.
    Serial.println("!!! ALERT: UNAUTHORIZED PRESENCE DETECTED !!!");

    /* 
     * ITERATIVE BLOCK: Police Siren Effect.
     * This 'for' loop runs 5 times to create a high-speed visual and acoustic alert.
     */
    for (int Cycle = 0; Cycle < 5; Cycle++) {

      // PHASE 1: RED ALERT
      // Calling 'fill' method to set all NeoPixels to pure Red.
      LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(255, 0, 0));
      // Calling hardware method to update the LED physical state.
      LastNameFirstName_10C.show();
      // Calling 'tone' function to generate a high-frequency alarm sound (2000 Hz).
      tone(BuzzerPin, 2000);
      // Execution pause to make the red light and sound perceptible.
      delay(150);

      // PHASE 2: BLUE ALERT
      // Calling 'fill' method to set all NeoPixels to pure Blue.
      LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(0, 0, 255));
      // Calling hardware method to refresh the LED array.
      LastNameFirstName_10C.show();
      // Calling 'noTone' function to create a pulse effect by silencing the buzzer.
      noTone(BuzzerPin);
      // Execution pause to complete the siren cycle.
      delay(150);
    }

  } else {
    /* 
     * ELSE BLOCK: Standby Monitoring State.
     * Executed when NO presence is detected, showing the system is armed.
     */

    // Calling hardware method to turn off all pixels in the array.
    LastNameFirstName_10C.clear();

    /* 
     * Calling 'setPixelColor' to light up ONLY the first LED (Index 0).
     * Color: Dim Red (50, 0, 0) to indicate the "Guard" is active without wasting power.
     */
    LastNameFirstName_10C.setPixelColor(0, 50, 0, 0);

    // Calling hardware method to update the physical state of the standby indicator.
    LastNameFirstName_10C.show();
  }
}

/* 
 * Function: RunSequentialTransition
 * Description: Creates a visual animation during tone transitions.
 */
void RunSequentialTransition(int R, int G, int B) {
  for (int i = 0; i < NumPixels; i++) {
    // Calling hardware methods to update specific pixels.
    LastNameFirstName_10C.setPixelColor(i, R, G, B);
    LastNameFirstName_10C.show();
    delay(50);
  }
}

/* 
 * Function: NotifySystemState
 * Description: Triple flash confirmation for system mode acknowledgment.
 */
void NotifySystemState(int R, int G, int B) {
  for (int Flash = 0; Flash < 3; Flash++) {
    // Calling hardware methods for visual and acoustic notification.
    LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(R, G, B));
    LastNameFirstName_10C.show();
    tone(BuzzerPin, 2500, 100);
    delay(200);
    LastNameFirstName_10C.clear();
    LastNameFirstName_10C.show();
    delay(200);
  }
}