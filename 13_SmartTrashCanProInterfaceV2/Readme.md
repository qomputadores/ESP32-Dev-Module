Project: 13_SmartTrashCanProInterfaceV2

1. Description

This project showcases a professional automotive-grade mechatronic waste disposal unit optimized for the ESP32 platform. It manages a hands-free lid deployment mechanism utilizing an FC-51 infrared proximity tracking array and a micro servo motor. Visual interactions are rendered via a high-definition geometric vector UI engine deployed on an SSD1306 OLED display using I2C communications. Additionally, an interactive audio layer is integrated via an active buzzer operating under advanced software frequency modulation: the acoustic generator sweeps down to grave frequencies during responsive opening cycles and elevates up to intense acute pitch patterns to emphasize pinch-point danger states during smooth mechatronic closures.

2. Learning Objectives

Acoustic Frequency Mapping: Program mathematical transpositions using map() variables to synchronize physical mechanical angles with dynamic audio frequencies.
Advanced Vector Graphing: Master the rendering of multi-point complex graphic primitives (triangles, round-rectangles, and arc-masks) on dot-matrix OLED systems.
Temporal Safety Synchronization: Coordinate concurrent hardware alerts (OLED display refreshing loops, audio siren patterns, and LED status changes) inside non-blocking routines.

3. How the "Actuation Logic" Works

The control loop monitors user hand presence metrics and advances execution flags continuously to cycle across specific mechatronic states:

System Phase | IR Sensor Path | OLED Face Interface | Hardware Green LED | Hardware Red LED | Acoustic Feedback Modulation
--- | --- | --- | --- | --- | ---
Standby Idle | Clear (1) | Stylized Happy Vector | ACTIVE (HIGH) | Inactive (LOW) | Muted Silenced Line
Lid Opening | Obstructed (0) | Shocked / Open Status | Inactive (LOW) | ACTIVE (HIGH) | Decreasing Frequency Sweep (1500Hz to 600Hz)
Hold Obstacle | Obstructed (0) | Suspicious Waiting Face| Inactive (LOW) | ACTIVE (HIGH) | Muted (Awaiting Clearance)
Safety Count | Clear (1) | Progress Bar + Digits | Inactive (LOW) | ACTIVE (HIGH) | Intermittent Sync Bleeps (1000Hz)
Controlled Close| Clear (1) | Flashing Danger Symbol | Inactive (LOW) | Toggle Remainder | Increasing Frequency Beep Sweep (600Hz to 2000Hz)

* Note on Graphic Interface Refactoring: Facial expressions now feature custom rounded irises, slanted eyebrow vectors, and masked smile matrices. The pinch point danger alert layout deploys an industrial double-nested perimeter triangle framing a stylized bold exclamation index.
* Note on Audio Physics Control: Closing cycles utilize modulo arithmetic remainder calculations ('Angle % 10 > 5') to toggle audio line states, slicing the rising frequency sweep into high-velocity sound bursts that increase pitch as the lid approaches its 0-degree limit.

4. Hardware Requirements

Microcontroller: ESP32 DevKit V1 (30-Pin layout).
Sensors/Inputs: 1x IR Obstacle Sensor Module (FC-51 active low).
Actuators/Outputs: 1x 0.96" OLED Display (Monochrome I2C), 1x SG90 Servo Motor, 1x Active Buzzer Module, 2x 5mm LEDs (Red, Green).
Resistors: 2x 220 Ω (for standard LED current protection limits).
Protoboard & Jumpers.

5. Connection Details (Data Only)

Component | Arduino Pin | Role
--- | --- | ---
OLED Display SDA Line | Pin 21 | Dedicated Hardware I2C Serial Data
OLED Display SCL Line | Pin 22 | Dedicated Hardware I2C Serial Clock
Servo Motor Signal | Pin 18 | Dedicated Software PWM Pulse Allocation
FC-51 Proximity Input | Pin 19 | Digital Signal Input Channel
Active Buzzer (+) | Pin 15 | Programmatic Audio Tone Frequency Drive
Green Standby LED | Pin 13 | Digital Output Line (Idle Indicator)
Red Alert LED | Pin 12 | Digital Output Line (Danger Indicator)

* Note on Peripheral Shielding: To shield the ESP32 logic core against electromagnetic backlash or current drops caused by the micro servo motor sweeps, install a 1000uF capacitor in parallel with the main protoboard power rails.

6. Telemetry Display Sequences

Operational diagnostics and structural state check strings are transmitted continuously over the serial port bus. Attach your device and run the Serial Monitor console at an exact speed profile of 115200 baud rate. The system will compile and render peripheral identity validation status fields and direct hardware print readouts highlighting real-time frequency calculations.

This material is for classroom use only. Commercial distribution is strictly prohibited.

Author: Iwan