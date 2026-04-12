Project 12: NeoPixel Fundamentals and Sequential Animations
1. Description
This project introduces the architecture of Addressable LEDs (NeoPixels). Unlike standard RGB systems, this project utilizes the WS2812B integrated circuit to control multiple lighting units through a single digital conductor. The focus is on understanding the Serial Data Protocol, timing requirements, and the implementation of non-blocking animation logic to create complex visual patterns such as wipes, chases, and rainbow cycles.

2. Hardware Anatomy (Physics/Theory)
Integrated Circuit WS2812B
Each NeoPixel is a combination of an electronic control circuit and an RGB chip.

Self-Clocked Protocol: Since there is no dedicated clock line (SCL), the communication relies on precise timing. A frequency of 800 kHz is required for the chip to correctly sample the high and low states of the data signal.

Data Reshaping: Each pixel consumes the first 24 bits of the incoming data stream and regenerates the remaining signal before passing it to the DOUT (Data Out) terminal, ensuring signal integrity across the chain.

Power Considerations
Current Peak: A single unit at maximum white intensity consumes ~60mA. Large arrays require external power injection to prevent voltage drops and damage to the microcontroller's regulator.

Logic Levels: While the ESP32 operates at 3.3V, NeoPixels are designed for 5V TTL logic. In long-distance data transmissions, a logic level shifter is necessary to ensure a stable high state.

3. Library Methods (Adafruit NeoPixel)
Adafruit_NeoPixel(count, pin, type): Class constructor to define the array size and hardware protocol (NEO_KHZ800).

Strip.begin(): Configures the designated GPIO for high-speed data output.

Strip.setPixelColor(index, color): Writes color data to the local buffer for a specific LED.

Strip.show(): Pushes the entire buffer to the physical LEDs via the serial bus.

Strip.setBrightness(0-255): Applies a global scaling factor to the PWM duty cycle of all channels.

4. Connection & Safety Protocols
Unidirectional Flow: The data signal must strictly enter through the DIN (Data Input) pad. Connecting to DOUT will result in communication failure.

Signal Damping: A resistor (330Ω - 470Ω) on the data line is mandatory to prevent impedance mismatch and destructive voltage reflections.

Common Ground: The power supply ground and the microcontroller ground must be physically tied to establish a common reference for the data signal.

Configuration: Users must update the LedCount constant to match the physical number of units in the connected stick or ring.

5. Connection Table
+-----------------------+------------------+-------------------+
|   COMPONENT PIN       |   ARDUINO/ESP32  |      FUNCTION     |
+-----------------------+------------------+-------------------+
|   NeoPixel VCC (5V)   |   5V / Ext PWR   |   Main Power      |
|   NeoPixel GND        |   GND            |   Common Ground   |
|   NeoPixel DIN        |   GPIO 14        |   Data Input      |
|   Data Resistor       |   In-line        |   Signal Protect  |
+-----------------------+------------------+-------------------+

This material is for classroom use only. Commercial distribution is strictly prohibited.

Author: Iwan