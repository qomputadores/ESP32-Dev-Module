/*
 * Author: Iwan
 * Project: 11_Smart_Trash_Can_Pro_Interface
 * Learning Objective: Implement advanced Mechatronic Control and UI/UX design 
 * utilizing object-identity verification on OLED hardware and dual-alert systems.
 */

#include <Wire.h>            // Standard I2C communication library.
#include <Adafruit_GFX.h>     // Core graphics library for geometric shapes and text.
#include <Adafruit_SSD1306.h>  // Hardware driver for SSD1306 OLED controller.
#include <ESP32Servo.h>       // Dedicated library for PWM-based servo control on ESP32.

// --- PREPROCESSOR MACROS ---
// Stringizing operator (#) to convert the student's identifier into a literal string.
#define GetName(Variable) #Variable

// --- OLED Hardware Configuration ---
#define ScreenWidth 128       // Physical width of the OLED display in pixels.
#define ScreenHeight 64       // Physical height of the OLED display in pixels.
#define OledAddress 0x3C      // I2C memory address for the display module.
#define OledReset -1          // No reset pin assigned (shared with ESP32 reset).

// --- STUDENT PERSONALIZATION AREA ---
// MANDATORY: The object name represents the student's identity. 
// Every call to the display hardware must use this specific identifier.
Adafruit_SSD1306 LastNameFirstName_11C(ScreenWidth, ScreenHeight, &Wire, OledReset);

// --- Hardware Pin Mapping ---
#define ServoPin 18           // Digital PWM output for the lid actuator.
#define ObstacleSensor 19     // Digital input for the FC-51 infrared sensor.
#define RedLed 12             // Output for critical warnings and closing state.
#define GreenLed 13           // Output for ready/standby visual indication.

// --- Global System Variables ---
Servo TrashServo;             // Instance of the servo object.
bool IsHandPresent = false;    // Stores the current state of the obstacle sensor.
bool IsLidOpen = false;        // Tracks the logical state of the lid mechanism.

void setup() {
  // Initialization of Serial telemetry for real-time debugging.
  Serial.begin(115200);

  // --- OLED INITIALIZATION ---
  // Passing the memory address of the I2C bus using the pointer '&Wire'.
  if(!LastNameFirstName_11C.begin(SSD1306_SWITCHCAPVCC, OledAddress)) {
    Serial.println(">> CRITICAL ERROR: OLED display hardware not found.");
    for(;;); // Safety loop to halt execution.
  }
  
  // --- INITIAL IDENTITY VERIFICATION ---
  // This phase validates that the student has correctly renamed the display object.
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  LastNameFirstName_11C.setCursor(0, 10);
  LastNameFirstName_11C.println("SYSTEM OWNER:");
  LastNameFirstName_11C.setCursor(0, 30);
  
  // Using the Macro to print the student's object name directly onto the hardware.
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C)); 
  LastNameFirstName_11C.display(); // Mandatory push from RAM buffer to physical panel.
  
  delay(3000); // 3-second delay for teacher verification during the lab session.

  // --- PERIPHERAL ATTACHMENT ---
  TrashServo.attach(ServoPin);
  pinMode(ObstacleSensor, INPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  
  // Default system state: Lid closed and green LED active.
  TrashServo.write(0);
  digitalWrite(GreenLed, HIGH);
  digitalWrite(RedLed, LOW);
  
  // Calling modular function to render the standby visual interface.
  ShowStatusFace("HAPPY"); 
}

void loop() {
  // 1. DATA ACQUISITION STAGE
  // '!' operator inverts the Active LOW logic from the FC-51 sensor.
  IsHandPresent = !digitalRead(ObstacleSensor);

  // 2. TRIGGER LOGIC: Evaluation of detection and lid state
  if (IsHandPresent && !IsLidOpen) {
    // Calling UI function for immediate visual feedback.
    ShowStatusFace("OPENING");
    
    // Fast actuation sequence (90 degrees) for responsive lid operation.
    TrashServo.write(90);
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    IsLidOpen = true;

    // 3. SAFETY BLOCKING LOOP
    // Prevents closing sequence while the hand remains in the sensor's path.
    while(!digitalRead(ObstacleSensor)) {
      ShowStatusFace("WAITING"); // Visual indicator of the active blocking state.
      delay(200);
    }

    // 4. TEMPORAL SAFETY COUNTDOWN
    // Notifies the user of the impending lid closure via visual progress bar.
    for (int i = 5; i > 0; i--) {
      ShowCountdown(i); // Calling the animation function for safety countdown.
      delay(1000);
    }

    // 5. MECHATRONIC CARE STAGE: Controlled Smooth Closing
    // Iterative loop to decrease the angle progressively, protecting the servo's gears.
    for (int Angle = 90; Angle >= 0; Angle--) {
      // Calling warning function to synchronize OLED blinking and Red LED.
      ShowWarning(Angle); 
      TrashServo.write(Angle);
      delay(30); // Defines the angular velocity of the closing mechanism.
    }
    
    // System restoration to idle/standby parameters.
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, HIGH);
    IsLidOpen = false;
    ShowStatusFace("HAPPY");
  }
  delay(100); // Sampling interval to maintain system stability.
}

// --- MODULAR UI (USER INTERFACE) FUNCTIONS ---

/* 
 * Function: ShowStatusFace
 * Description: Displays a character-based UI to represent the system's emotional state.
 */
void ShowStatusFace(String State) {
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  LastNameFirstName_11C.setCursor(0,0);
  
  // Watermark: The student's object identifier remains visible for validation.
  LastNameFirstName_11C.print("OWNER: ");
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C));

  if (State == "HAPPY") {
    // Basic standby facial expression.
    LastNameFirstName_11C.fillCircle(44, 40, 5, WHITE); // Left Eye.
    LastNameFirstName_11C.fillCircle(84, 40, 5, WHITE); // Right Eye.
    LastNameFirstName_11C.drawFastHLine(54, 55, 20, WHITE); // Mouth.
  } 
  else if (State == "OPENING") {
    // Reaction face for obstacle detection.
    LastNameFirstName_11C.drawCircle(44, 40, 8, WHITE); // Surprised L-Eye.
    LastNameFirstName_11C.drawCircle(84, 40, 8, WHITE); // Surprised R-Eye.
    LastNameFirstName_11C.setCursor(45, 50);
    LastNameFirstName_11C.println("OPEN!");
  }
  else if (State == "WAITING") {
    // Alert face for persistence detection.
    LastNameFirstName_11C.fillCircle(44, 40, 5, WHITE);
    LastNameFirstName_11C.drawFastHLine(74, 40, 20, WHITE); // Wink/Blink.
    LastNameFirstName_11C.setCursor(35, 55);
    LastNameFirstName_11C.println("WAITING...");
  }
  
  LastNameFirstName_11C.display();
}

/* 
 * Function: ShowCountdown
 * Description: Renders a dynamic bar to visualize the time remaining before closure.
 */
void ShowCountdown(int Seconds) {
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setCursor(0, 0);
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C)); 
  
  LastNameFirstName_11C.setCursor(35, 15);
  LastNameFirstName_11C.println("CLOSING IN:");
  
  // Progressive bar logic: Width = Seconds multiplied by a scaling factor.
  LastNameFirstName_11C.drawRect(14, 30, 100, 10, WHITE);
  LastNameFirstName_11C.fillRect(14, 30, Seconds * 20, 10, WHITE);
  
  LastNameFirstName_11C.setTextSize(2);
  LastNameFirstName_11C.setCursor(55, 45);
  LastNameFirstName_11C.println(Seconds);
  
  LastNameFirstName_11C.display();
}

/* 
 * Function: ShowWarning
 * Description: Implements a synchronized blinking warning (OLED Icon + Hardware LED).
 */
void ShowWarning(int CurrentAngle) {
  LastNameFirstName_11C.clearDisplay();
  
  // Blinking algorithm: Modulo operator creates a toggle effect based on the angle.
  if (CurrentAngle % 10 > 5) {
    // Drawing the Danger Triangle (Pinch Point warning).
    LastNameFirstName_11C.drawTriangle(64, 5, 34, 50, 94, 50, WHITE);
    LastNameFirstName_11C.fillRoundRect(62, 15, 4, 20, 2, WHITE); // Exclamation mark body.
    LastNameFirstName_11C.fillCircle(64, 43, 2, WHITE); // Exclamation mark dot.
    
    // Hardware alert: Red LED synchronized with the graphical icon.
    digitalWrite(RedLed, HIGH);
  } else {
    digitalWrite(RedLed, LOW); // Turning off the hardware alert during the blink cycle.
  }
  
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setCursor(15, 55);
  LastNameFirstName_11C.print("CAUTION: ");
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C));
  
  LastNameFirstName_11C.display();
}