/*
 * Author: Iwan
 * Project: 12 Smart Night Light Security Agent V2
 * Learning Objective: Implement advanced mechatronic flow control using Switch Case 
 * structures, ESP32 hardware timers allocation, and non-blocking lamp stabilization.
 *
 * This material is for classroom use only. 
 * Commercial distribution is strictly prohibited.
 */

/*
 * +-------------------------------------------------------------------------+
 * |                     TECHNICAL CONNECTION MAP (DATA ONLY)                |
 * +-----------------------+-------------------+-----------------------------+
 * |   HARDWARE COMPONENT  | ARDUINO PIN (ESP32|         PIN TYPE            |
 * +-----------------------+-------------------+-----------------------------+
 * |   LDR Light Sensor    | G32 (GPIO32)      | Analog Input (ADC1_CH4)     |
 * |   FC-51 Infrared      | G33 (GPIO33)      | Digital Input Channel       |
 * |   WS2812B NeoPixel    | G13 (GPIO13)      | Digital Output (Data Link)  |
 * |   Active Buzzer Module| G15 (GPIO15)      | Digital Output Channel       |
 * |   Servo Motor SG90    | G14 (GPIO14)      | Dedicated Servo PWM Output  |
 * +-----------------------+-------------------+-----------------------------+
 */

#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>

// --- PREPROCESSOR MACROS ---
#define GetName(Variable) #Variable

// --- Hardware Mapping Parameters ---
#define LdrPin 32        
#define PresencePin 33   
#define NeoPixelPin 13   
#define BuzzerPin 15     
#define ServoPin 14      
#define NumPixels 12     

// --- STUDENT PERSONALIZATION AREA ---
Adafruit_NeoPixel LastNameFirstName_10C(NumPixels, NeoPixelPin, NEO_GRB + NEO_KHZ800);

// --- Object Initialization ---
Servo GateServo;

// --- Global Control Variables ---
int LdrValue = 0;
int IntrudersCount = 0;
char CurrentMode = 'L';  // Operational baseline: 'L' = Lamp / 'S' = Security
bool IsWarmTone = true;  
bool PresenceDetected = false;
bool IsLampOn = false;   // Software latch flag: guarantees steady lighting without flickering

void setup() {
  // --- TELEMETRY INITIALIZATION ---
  Serial.begin(115200);

  // --- ESP32 DEDICATED PWM TIMER ALLOCATION ---
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // --- ACTUATOR CONFIGURATION & HOME ALIGNMENT ---
  GateServo.setPeriodHertz(50); // Standard 50Hz pulse frequency for SG90 servos
  GateServo.attach(ServoPin, 500, 2400);
  GateServo.write(180); // Default open state when system boots in Lamp Mode

  // --- PIN CONFIGURATION ---
  pinMode(LdrPin, INPUT);
  pinMode(PresencePin, INPUT);
  pinMode(BuzzerPin, OUTPUT);

  // --- NEOPIXEL INITIALIZATION ---
  LastNameFirstName_10C.begin();
  LastNameFirstName_10C.setBrightness(100); 
  LastNameFirstName_10C.show();

  // --- STARTUP FEEDBACK ---
  Serial.println("--- SYSTEM INITIALIZED: SWITCH CASE LOGIC ---");
  Serial.print("Project Owner Identifier: ");
  Serial.println(GetName(LastNameFirstName_10C));
  Serial.println("--------------------------------------------");
  Serial.println("Commands: 'L' (Lamp), 'S' (Security), 'W' (Warm), 'C' (Cold)\n");
}

void loop() {
  // --- 1. SENSOR DATA ACQUISITION ---
  LdrValue = analogRead(LdrPin);
  
  // Presence sensor uses active LOW logic (0 when detecting); '!' inverts it
  PresenceDetected = !digitalRead(PresencePin);

  // --- 2. SERIAL INTERFACE HANDLING ---
  if (Serial.available() > 0) {
    char IncomingByte = Serial.read();
    HandleSystemModes(IncomingByte);
  }

  // --- 3. EXECUTION SELECTOR (STATE MACHINE) ---
  switch (CurrentMode) {
    case 'L':
      ExecuteLampMode();
      break;

    case 'S':
      ExecuteSecurityMode();
      break;

    default:
      LastNameFirstName_10C.clear();
      LastNameFirstName_10C.show();
      break;
  }

  delay(50);
}

// --- MODULAR LOGIC FUNCTIONS ---

void HandleSystemModes(char Command) {
  switch (Command) {
    case 'S':
    case 's':
      CurrentMode = 'S';
      IntrudersCount = 0;
      
      // Force immediate physical lockdown closure when security arms
      GateServo.write(0); 
      
      NotifySystemState(255, 165, 0); // Orange flash confirmation
      Serial.println(">> MODE: SECURITY AGENT ARMED - GATE LOCKED CLOSED (0 DEG)");
      break;

    case 'L':
    case 'l':
      CurrentMode = 'L';
      IsLampOn = false; // Reset lamp latch flag to allow immediate LDR assessment
      
      // Open the residential barrier for regular operation mode
      GateServo.write(180); 
      
      NotifySystemState(0, 0, 255); // Blue flash confirmation
      Serial.print(">> MODE: SMART LAMP ACTIVE (GATE OPEN 180 DEG). Intrusions: ");
      Serial.println(IntrudersCount);
      break;

    case 'W':
    case 'w':
      if (CurrentMode == 'L') {
        IsWarmTone = true;
        IsLampOn = false; // Reset latch to apply new color instantly without flashing
        RunSequentialTransition(255, 100, 20);
        Serial.println(">> TONE: WARM SPECTRUM SELECTED");
      } else {
        Serial.println(">> ACCESS DENIED: Cannot change tone in Security Mode.");
        tone(BuzzerPin, 300, 200);
      }
      break;

    case 'C':
    case 'c':
      if (CurrentMode == 'L') {
        IsWarmTone = false;
        IsLampOn = false; // Reset latch to apply new color instantly without flashing
        RunSequentialTransition(200, 200, 255);
        Serial.println(">> TONE: COLD SPECTRUM SELECTED");
      } else {
        Serial.println(">> ACCESS DENIED: Cannot change tone in Security Mode.");
        tone(BuzzerPin, 300, 200);
      }
      break;

    default:
      if (Command != '\n' && Command != '\r') {
        Serial.println(">> ERROR: Protocol command not recognized.");
        tone(BuzzerPin, 400, 200);
      }
      break;
  }
}

void ExecuteLampMode() {
  // Check if environmental darkness threshold is breached (Darkness on ESP32 ADC)
  if (LdrValue > 3000) {
    // Structural Guard: Execute smooth fade-in only if the lamp is not currently on
    if (!IsLampOn) {
      for (int Brightness = 0; Brightness <= 150; Brightness += 5) {
        if (IsWarmTone) {
          // Warm Sunset Spectrum Mix Ratio (Red dominant, Green halved, Blue minimal)
          LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(Brightness, Brightness / 2, Brightness / 10));
        } else {
          // Balanced White Spectrum Mix Ratio
          LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(Brightness, Brightness, Brightness));
        }
        LastNameFirstName_10C.show();
        delay(10);
      }
      IsLampOn = true; // Lock illumination state to prevent the loop from refading and flashing
    }
  } else {
    // If external light returns, clear the hardware and release software latch
    if (IsLampOn) {
      LastNameFirstName_10C.clear();
      LastNameFirstName_10C.show();
      IsLampOn = false;
    }
  }
}

void ExecuteSecurityMode() {
  if (PresenceDetected) {
    IntrudersCount++;
    Serial.println("!!! ALERT: UNAUTHORIZED PRESENCE DETECTED !!!");

    // Dynamic Police Siren routine (5 high-speed strobe cycles)
    for (int Cycle = 0; Cycle < 5; Cycle++) {
      // PHASE 1: PURE RED CHRONO FLASH
      LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(255, 0, 0));
      LastNameFirstName_10C.show();
      tone(BuzzerPin, 2000);
      delay(150);

      // PHASE 2: PURE BLUE CHRONO FLASH
      LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(0, 0, 255));
      LastNameFirstName_10C.show();
      noTone(BuzzerPin);
      delay(150);
    }
  } else {
    // Standard Secure Guard Active State (Standby mode)
    LastNameFirstName_10C.clear();
    
    // Set a steady, dim red sentinel heartbeat indicator on the first pixel (index 0)
    LastNameFirstName_10C.setPixelColor(0, 50, 0, 0);
    LastNameFirstName_10C.show();
  }
}

void RunSequentialTransition(int R, int G, int B) {
  for (int i = 0; i < NumPixels; i++) {
    LastNameFirstName_10C.setPixelColor(i, R, G, B);
    LastNameFirstName_10C.show();
    delay(50);
  }
}

void NotifySystemState(int R, int G, int B) {
  for (int Flash = 0; Flash < 3; Flash++) {
    LastNameFirstName_10C.fill(LastNameFirstName_10C.Color(R, G, B));
    LastNameFirstName_10C.show();
    tone(BuzzerPin, 2500, 100);
    delay(200);
    LastNameFirstName_10C.clear();
    LastNameFirstName_10C.show();
    delay(200);
  }
}