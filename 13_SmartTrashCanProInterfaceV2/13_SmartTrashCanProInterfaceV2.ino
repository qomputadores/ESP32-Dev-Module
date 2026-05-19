/*
 * Author: Iwan
 * Project: 13 Smart Trash Can Pro Interface V2
 * Learning Objective: Implement advanced Mechatronic Control, interactive acoustic 
 * frequency modulation, and professional vector UI/UX design on OLED hardware.
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
 * |   OLED Display SDA    | G21 (GPIO21)      | Dedicated I2C Data Bus      |
 * |   OLED Display SCL    | G22 (GPIO22)      | Dedicated I2C Clock Bus     |
 * |   Servo Motor Signal  | G18 (GPIO18)      | Dedicated Servo PWM Output  |
 * |   FC-51 IR Sensor     | G19 (GPIO19)      | Digital Input Channel       |
 * |   Active Buzzer Module| G15 (GPIO15)      | Dedicated Audio PWM Output  |
 * |   Red Alert LED       | G12 (GPIO12)      | Digital Output Channel       |
 * |   Green Standby LED   | G13 (GPIO13)      | Digital Output Channel       |
 * +-----------------------+-------------------+-----------------------------+
 */

#include <Wire.h>            
#include <Adafruit_GFX.h>     
#include <Adafruit_SSD1306.h>  
#include <ESP32Servo.h>       

// --- PREPROCESSOR MACROS ---
#define GetName(Variable) #Variable

// --- OLED Hardware Configuration ---
#define ScreenWidth 128       
#define ScreenHeight 64       
#define OledAddress 0x3C      
#define OledReset -1          

// --- STUDENT PERSONALIZATION AREA ---
Adafruit_SSD1306 LastNameFirstName_11C(ScreenWidth, ScreenHeight, &Wire, OledReset);

// --- Hardware Pin Mapping ---
#define ServoPin 18           
#define ObstacleSensor 19     
#define RedLed 12             
#define GreenLed 13           
#define BuzzerPin 15          // Integrated Active Buzzer channel

// --- Global System Variables ---
Servo TrashServo;             
bool IsHandPresent = false;    
bool IsLidOpen = false;        

void setup() {
  // Initialization of Serial telemetry for real-time debugging
  Serial.begin(115200);

  // --- OLED INITIALIZATION ---
  if(!LastNameFirstName_11C.begin(SSD1306_SWITCHCAPVCC, OledAddress)) {
    Serial.println(">> CRITICAL ERROR: OLED display hardware not found.");
    for(;;); 
  }
  
  // --- INITIAL IDENTITY VERIFICATION ---
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  LastNameFirstName_11C.setCursor(0, 10);
  LastNameFirstName_11C.println("SYSTEM_OWNER:");
  LastNameFirstName_11C.setCursor(0, 30);
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C)); 
  LastNameFirstName_11C.display(); 
  
  delay(3000); // 3-second delay for teacher verification

  // --- PERIPHERAL ATTACHMENT ---
  TrashServo.attach(ServoPin);
  pinMode(ObstacleSensor, INPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  
  // Default system state initialization
  TrashServo.write(0);
  digitalWrite(GreenLed, HIGH);
  digitalWrite(RedLed, LOW);
  noTone(BuzzerPin);
  
  ShowStatusFace("HAPPY"); 
}

void loop() {
  // 1. DATA ACQUISITION STAGE
  IsHandPresent = !digitalRead(ObstacleSensor);

  // 2. TRIGGER LOGIC: Evaluation of detection and lid state
  if (IsHandPresent && !IsLidOpen) {
    ShowStatusFace("OPENING");
    
    // --- INTEGRATED AUDIO EFFECT: OPENING PITTS ---
    // Short confirmation chirp before moving mechanical gears
    tone(BuzzerPin, 1800, 80); 
    delay(80);
    noTone(BuzzerPin);

    // Controlled opening sweep with decreasing acoustic frequency modulation
    for (int Angle = 0; Angle <= 90; Angle += 2) {
      TrashServo.write(Angle);
      // Frequency decreases dynamically from 1500Hz down to 600Hz as lid opens
      int OpenFreq = map(Angle, 0, 90, 1500, 600);
      tone(BuzzerPin, OpenFreq);
      delay(10); 
    }
    noTone(BuzzerPin); // Safe audio shutdown

    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    IsLidOpen = true;

    // 3. SAFETY BLOCKING LOOP
    while(!digitalRead(ObstacleSensor)) {
      ShowStatusFace("WAITING"); 
      delay(200);
    }

    // 4. TEMPORAL SAFETY COUNTDOWN
    for (int i = 5; i > 0; i--) {
      ShowCountdown(i); 
      // Synchronized interactive countdown bleep
      tone(BuzzerPin, 1000, 100);
      delay(1000);
    }

    // 5. MECHATRONIC CARE STAGE: Controlled Smooth Closing
    for (int Angle = 90; Angle >= 0; Angle--) {
      ShowWarning(Angle); 
      TrashServo.write(Angle);
      
      // --- INTEGRATED AUDIO EFFECT: CLOSING ALERT ---
      // Frequency increases dynamically from 600Hz up to 2000Hz as danger closes
      int CloseFreq = map(Angle, 90, 0, 600, 2000);
      
      // Modulo logic creates intermittent high-speed tactical beeps synced with motion
      if (Angle % 10 > 5) {
        tone(BuzzerPin, CloseFreq);
      } else {
        noTone(BuzzerPin);
      }
      
      delay(30); // Defines the angular velocity of the closing mechanism
    }
    noTone(BuzzerPin); // Secure audio line cutoff
    
    // System restoration to idle/standby parameters
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, HIGH);
    IsLidOpen = false;
    ShowStatusFace("HAPPY");
  }
  delay(100); 
}

// --- MODULAR UI (USER INTERFACE) FUNCTIONS ---

void ShowStatusFace(String State) {
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  
  // Persistent Owner Watermark Layout
  LastNameFirstName_11C.setCursor(0,0);
  LastNameFirstName_11C.print("UI_OWNER: ");
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C));

  if (State == "HAPPY") {
    // Advanced Stylized Happy Expression
    LastNameFirstName_11C.fillRoundRect(35, 30, 14, 8, 4, WHITE);  // Left Eye
    LastNameFirstName_11C.fillRoundRect(79, 30, 14, 8, 4, WHITE);  // Right Eye
    LastNameFirstName_11C.drawFastHLine(33, 26, 18, WHITE);        // Left Relaxed Eyebrow
    LastNameFirstName_11C.drawFastHLine(77, 26, 18, WHITE);        // Right Relaxed Eyebrow
    
    // Geometric Curved Mouth using precise vector lines
    LastNameFirstName_11C.drawRoundRect(49, 46, 30, 12, 6, WHITE); 
    LastNameFirstName_11C.fillRect(49, 44, 30, 7, BLACK);          // Mask upper half to render smile
  } 
  else if (State == "OPENING") {
    // Highly Advanced Shocked/Surprised Face
    LastNameFirstName_11C.drawCircle(42, 34, 10, WHITE);           // Extended Left Eye Ring
    LastNameFirstName_11C.fillCircle(42, 34, 4, WHITE);            // Left Iris focus
    LastNameFirstName_11C.drawCircle(86, 34, 10, WHITE);           // Extended Right Eye Ring
    LastNameFirstName_11C.fillCircle(86, 34, 4, WHITE);            // Right Iris focus
    
    // Slanted Expressive Dynamic Eyebrows
    LastNameFirstName_11C.drawLine(30, 20, 50, 25, WHITE);        // Angled Left Eyebrow
    LastNameFirstName_11C.drawLine(98, 20, 78, 25, WHITE);        // Angled Right Eyebrow
    
    LastNameFirstName_11C.drawCircle(64, 52, 7, WHITE);            // O-shaped Surprised Mouth Vector
  }
  else if (State == "WAITING") {
    // Focused Warning Detection Layout Face
    LastNameFirstName_11C.fillRoundRect(35, 34, 14, 5, 1, WHITE);  // Steady Left Suspicious Eye
    LastNameFirstName_11C.drawLine(77, 30, 93, 38, WHITE);        // Winking Right Eye Line 1
    LastNameFirstName_11C.drawLine(77, 38, 93, 30, WHITE);        // Winking Right Eye Line 2
    
    LastNameFirstName_11C.drawLine(33, 27, 49, 29, WHITE);        // Lowered Left Eyebrow
    LastNameFirstName_11C.drawFastHLine(54, 48, 20, WHITE);        // Flat Neutral Line Mouth
  }
  
  LastNameFirstName_11C.display();
}

void ShowCountdown(int Seconds) {
  LastNameFirstName_11C.clearDisplay();
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  
  LastNameFirstName_11C.setCursor(0, 0);
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C)); 
  
  LastNameFirstName_11C.setCursor(32, 12);
  LastNameFirstName_11C.println("AUTO-CLOSING:");
  
  // Professional Segmented Progress Bar Render Layout
  LastNameFirstName_11C.drawRoundRect(14, 26, 100, 12, 3, WHITE);
  LastNameFirstName_11C.fillRoundRect(16, 28, Seconds * 19, 8, 2, WHITE);
  
  // Focal Large Countdown Typography
  LastNameFirstName_11C.setTextSize(3);
  LastNameFirstName_11C.setCursor(56, 42);
  LastNameFirstName_11C.println(Seconds);
  
  LastNameFirstName_11C.display();
}

void ShowWarning(int CurrentAngle) {
  LastNameFirstName_11C.clearDisplay();
  
  // High-Visibility Pro Double Triangle Danger Vector Symbol
  if (CurrentAngle % 10 > 5) {
    // Outer Border Warning Triangle
    LastNameFirstName_11C.drawTriangle(64, 2, 30, 48, 98, 48, WHITE);
    // Inner Offset Warning Triangle for structural bold thickness
    LastNameFirstName_11C.drawTriangle(64, 5, 33, 46, 95, 46, WHITE);
    
    // Stylized Industrial Exclamation Mark
    LastNameFirstName_11C.fillRoundRect(62, 16, 5, 18, 2, WHITE); 
    LastNameFirstName_11C.fillCircle(64, 40, 3, WHITE);           
    
    digitalWrite(RedLed, HIGH);
  } else {
    digitalWrite(RedLed, LOW); 
  }
  
  LastNameFirstName_11C.setTextSize(1);
  LastNameFirstName_11C.setTextColor(WHITE);
  LastNameFirstName_11C.setCursor(12, 54);
  LastNameFirstName_11C.print("PINCH POINT: ");
  LastNameFirstName_11C.println(GetName(LastNameFirstName_11C));
  
  LastNameFirstName_11C.display();
}