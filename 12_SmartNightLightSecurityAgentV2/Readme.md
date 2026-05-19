Project: 12_SmartNightLightSecurityAgentV2

1. Description

This project simulates an automated mechatronic security agent and smart night light system optimized for the ESP32 architecture. It features a dual-state control machine that alternates between a Smart Lamp mode and an Armed Security Agent. In Smart Lamp mode, an LDR sensor drives a 12-LED NeoPixel matrix ring, providing a continuous, steady ambient illumination (Warm or Cold spectrum) without flickering. Upon switching to Security Mode, a micro servo motor automatically moves to 0 degrees to lock down a perimeter gate, while an FC-51 infrared presence sensor tracks intrusion counts and dispatches a high-speed auditory and visual police siren sequence.

2. Learning Objectives

Mechatronic Lockdowns: Synchronize physical barrier motor repositioning commands with structural security state transitions.
Flicker Mitigation Logic: Implement tracking software flags to prevent recursive dimming loops inside high-frequency processing sequences.
Platform Architecture Allocation: Understand internal ESP32 resource allocation by programmatically provisioning PWM hardware timers for servo control.

3. How the "Actuation Logic" Works

The system utilizes structural serial command processing to jump between operation states, altering peripheral behaviors based on active parameters:

Mode | Command | LDR Threshold | Presence Sensor (FC-51) | NeoPixel Light Matrix | Servo Gate Position
--- | --- | --- | --- | --- | ---
Smart Lamp | 'L' / 'l' | Dark (>3000) | Bypassed | Steady Warm/Cold Fade-In | Open (180 deg)
Smart Lamp | 'L' / 'l' | Bright (<3000)| Bypassed | Fully Extinguished | Open (180 deg)
Security Agent | 'S' / 's' | Bypassed | Safe (No Target) | Sentinel Dim Red Pixel (0) | Locked Closed (0 deg)
Security Agent | 'S' / 's' | Bypassed | Alarm (Target Detected) | Red/Blue High-Speed Siren | Locked Closed (0 deg)

* Note on Flicker Mitigation Protocol: The software uses a boolean latch flag ('IsLampOn') to control the smart lamp block. When darkness is detected, the progressive fade-in loop executes exactly once, preventing the system from restarting the fade routine on every loop cycle, ensuring a rigid, non-oscillating lighting stream.
* Note on Security Intrusions: Transitioning into Security mode flushes the intrusion counter buffer. Every target crossing event detected by the active-low infrared module increments the database and loops an alternate 2000Hz tone blink block 5 consecutive times.

4. Hardware Requirements

[cite_start]Microcontroller: ESP32 DevKit V1 (30-Pin layout)[cite: 3].
[cite_start]Sensors/Inputs: 1x Light Sensor Module (LDR) [cite: 3][cite_start], 1x Infrared Sensor Module FC-51[cite: 3].
[cite_start]Actuators/Outputs: 1x NeoPixel Module Ring WS2812B (12 LEDs) [cite: 3][cite_start], 1x SG90 Servo Motor [cite: 3][cite_start], 1x Active Buzzer Module[cite: 3].
[cite_start]Protoboard & Jumpers[cite: 3].

5. Connection Details (Data Only)

Component | Arduino Pin | Role
--- | --- | ---
LDR Light Sensor | Pin 32 | Analog ADC1 Input Channel
FC-51 Infrared Sensor | Pin 33 | Digital Input Channel
WS2812B Data Link | Pin 13 | High-Speed Digital Output Line
Active Buzzer (+) | Pin 15 | Digital Command Output Line
Servo Motor Signal | Pin 14 | Allocated Hardware PWM Timer Output

* Note on Resource Allocation: To achieve smooth hardware PWM servo motion on the ESP32 without messing up NeoPixel data lines, the setup routine allocates timers 0 through 3 explicitly before anchoring the servo output on GPIO pin 14.

6. Telemetry Display Sequences

Operational variables and calibration logs are transmitted via the hardware serial channel. Open the Serial Monitor terminal and set the communication speed to 115200 baud. The interface will display startup validations, project owner ownership strings, and input menus. Send commands ('L', 'S', 'W', 'C') to manipulate the runtime states and review intruder tally database reports dynamically.

This material is for classroom use only. Commercial distribution is strictly prohibited.

Author: Iwan