/*
 * Author: Iwan
 * Project: 09 NeoPixel Fundamentals
 * Hardware: WS2812B (Ring or Stick) + ESP32/Arduino
 * Description: Basic testing suite with 4 distinct animations.
 */

#include <Adafruit_NeoPixel.h>

// --- USER CONFIGURATION (MODIFY HERE) ---
#define DataPin      14    // Data output pin
#define LedCount     12    // <--- STUDENT: SET THE NUMBER OF PIXELS IN YOUR DEVICE
#define Brightness   50    // Brightness level (0 to 255) to manage power draw

// NeoPixel object declaration
Adafruit_NeoPixel Strip(LedCount, DataPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Strip.begin();           // Initialize hardware pins
  Strip.show();            // Turn off all pixels initially
  Strip.setBrightness(Brightness); 
}

void loop() {
  // ANIMATION 1: Color Wipe (Sequential filling)
  // Parameters: (Color in RGB format, Delay time)
  ShowColorWipe(Strip.Color(255, 0, 0), 50); // Red
  ShowColorWipe(Strip.Color(0, 255, 0), 50); // Green
  ShowColorWipe(Strip.Color(0, 0, 255), 50); // Blue

  // ANIMATION 2: Theater Chase (Marquee effect)
  // Parameters: (Color in RGB format, Delay time)
  ShowTheaterChase(Strip.Color(127, 127, 127), 50); // Soft White

  // ANIMATION 3: Rainbow (Static spectrum)
  ShowRainbow(20);

  // ANIMATION 4: Rainbow Cycle (Flowing spectrum)
  ShowRainbowCycle(20);
}

// --- ANIMATION FUNCTIONS ---

// Fills pixels one by one
void ShowColorWipe(uint32_t Color, int Wait) {
  for(int i=0; i < Strip.numPixels(); i++) {
    Strip.setPixelColor(i, Color);
    Strip.show();
    delay(Wait);
  }
}

// Marquee-style flashing lights
void ShowTheaterChase(uint32_t Color, int Wait) {
  for (int j=0; j < 10; j++) { 
    for (int q=0; q < 3; q++) {
      for (int i=0; i < Strip.numPixels(); i=i+3) {
        Strip.setPixelColor(i+q, Color);
      }
      Strip.show();
      delay(Wait);
      for (int i=0; i < Strip.numPixels(); i=i+3) {
        Strip.setPixelColor(i+q, 0); 
      }
    }
  }
}

// Displays a rainbow across the entire strip
void ShowRainbow(int Wait) {
  uint16_t i, j;
  for(j=0; j < 256; j++) {
    for(i=0; i < Strip.numPixels(); i++) {
      Strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    Strip.show();
    delay(Wait);
  }
}

// Creates a flowing rainbow effect
void ShowRainbowCycle(int Wait) {
  uint16_t i, j;
  for(j=0; j < 256*5; j++) { 
    for(i=0; i < Strip.numPixels(); i++) {
      Strip.setPixelColor(i, Wheel(((i * 256 / Strip.numPixels()) + j) & 255));
    }
    Strip.show();
    delay(Wait);
  }
}

// Helper function to generate smooth colors (Color Wheel)
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}