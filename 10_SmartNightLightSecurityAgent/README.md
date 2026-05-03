# Project 10: Smart Night Light & Security Agent

## Description
This project implements an intelligent dual-state system using the ESP32 platform. It integrates a WS2812B NeoPixel array, an active buzzer, and sensory inputs (LDR and FC-51) to switch between a sophisticated automated lamp and a high-alert security agent. The system is controlled via a Serial Interface, utilizing a robust state machine architecture to ensure operational reliability and hierarchical command validation.

---

## Operating Manual: User Experience
To ensure the system is functioning according to technical specifications, follow this operational sequence:

1.  **Initialization**: Upon power-up, the system initializes all peripherals. The Serial Monitor will display the **Project Owner Identifier** (extracted directly from the object name) and the available command protocol.
2.  **Smart Lamp Mode (Default)**:
    *   **Automation**: If the ambient light drops below the calibrated threshold ($LdrValue > 3000$), the NeoPixel array will execute a smooth **Dimming** effect, increasing brightness progressively to 150 (8-bit scale).
    *   **Tone Control**: Send 'W' for a Warm spectrum or 'C' for a Cold spectrum. The system will animate the transition and update the visual output.
3.  **Security Agent Mode**:
    *   **Activation**: Send 'S' via the Serial Monitor. The system will flash Amber and enter the monitoring state.
    *   **Detection**: If the FC-51 sensor detects movement, a high-frequency (2000 Hz) acoustic alarm and a Red/Blue visual siren will trigger for 5 cycles.
    *   **Command Validation**: If you attempt to change the light tone ('W' or 'C') while the Security Agent is armed, the system will deny access, protecting the integrity of the security protocol.

---

## Technical Theory: Advanced Control Flow

### Switch Case Structure
The `switch` statement is a powerful control structure used to select one of many code blocks to be executed. In this project, it acts as a **State Selector**.
*   **Efficiency**: Instead of evaluating multiple `if-else` conditions sequentially, the `switch` jumps directly to the matching `case`, optimizing processor cycles.
*   **Safety**: The `default` case acts as a security net, handling any unexpected or unrecognized input data.

### Modular Programming (Functions)
Modularization is the process of breaking a complex program into smaller, self-contained sub-programs or **Functions**.
*   **Hierarchy**: The `void loop()` acts as a manager, simply "calling" specific functions like `ExecuteSecurityMode()` or `ExecuteLampMode()` when needed.
*   **Readability**: By separating the "Detection" logic from the "Action" logic, the code becomes professional, organized, and easier to debug for high-level engineering tasks.

---

## Hardware Anatomy (Physics/Theory)
*   **LDR (Light Dependent Resistor)**: A photoresistor whose resistance decreases as the intensity of incident light increases, allowing the ESP32 to measure environmental LUX through a voltage divider.
*   **WS2812B Protocol**: A single-wire communication protocol where each LED contains an integrated driver. It relies on precise timing (800 kHz) to interpret 24-bit color data (8 bits per channel: R, G, B).
*   **Active Buzzer**: A piezoelectric transducer with an internal oscillating circuit. It requires only a high/low signal to produce a fixed-frequency sound.

---

## Power/Safety Protocols
*   **Current Limiting**: The NeoPixel brightness is capped at 150/255 to prevent excessive current draw from the ESP32 internal regulator.
*   **Common Ground**: All components must share a common GND rail to ensure a stable reference for signal processing.
*   **Voltage Rails**: Ensure the NeoPixel array is powered by the 5V/VIN pin, as the 3.3V rail may cause signal instability.

---

## Connection Table

| COMPONENT | ARDUINO PIN | NOTES |
| :--- | :--- | :--- |
| LDR Sensor | 32 | Analog Input (10k Pull-down) |
| FC-51 Sensor | 33 | Digital Input (Active LOW) |
| NeoPixel DI | 13 | Data Signal (WS2812B) |
| Active Buzzer | 15 | Digital Output (Acoustic Alert) |

---

This material is for classroom use only. 
Commercial distribution is strictly prohibited.
Author: Iwan.