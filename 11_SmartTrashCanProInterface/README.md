# Project: Smart Trash Can Pro - Mechatronic System

## Description
This project focuses on the integration of a mechatronic system capable of automated waste management and real-time user feedback. By combining an ESP32 microcontroller with infrared sensing and high-precision servo actuation, the system executes a safe and controlled mechanical cycle. The inclusion of an SSD1306 OLED display allows the implementation of a professional User Interface (UI), which communicates the system's internal states through geometric animations and technical icons.

---

## Hardware Anatomy (Physics/Theory)

### 1. SSD1306 OLED Display (The Visual Interface)
*   **Technology**: Organic Light-Emitting Diode. It provides high contrast and low power consumption by only activating individual pixels as needed.
*   **Communication**: It utilizes the I2C (Inter-Integrated Circuit) protocol, requiring only two signals: SDA (Serial Data) and SCL (Serial Clock).
*   **Coordinate System**: The display operates on a grid of 128 (X) by 64 (Y) pixels, where (0,0) is the top-left corner.

### 2. FC-51 Infrared Sensor (The Perception Unit)
*   **Principle**: Light reflection. An IR emitter sends a beam; if an object is present, the receiver detects the reflected light.
*   **Logic**: It operates on Active LOW logic, where a '0' signal indicates object detection.

### 3. Servomotor SG90/MG90S (The Actuator)
*   **Control**: Managed through Pulse Width Modulation (PWM). The duty cycle of the signal determines the specific angular position of the lid (from 0° to 90°).

---

## Technical Functionality & Operational Logic

1.  **System Initialization**: The OLED performs a hardware check and displays the owner's identification for session validation.
2.  **Detection & Response**: Upon obstacle detection, the system triggers a "Fast Opening" sequence. Simultaneously, the UI updates to an "Active" state with surprised-eye animations.
3.  **Safety Blocking (Non-Interruptible Loop)**: A safety algorithm prevents the lid from closing as long as the infrared sensor detects an object, ensuring zero-risk interaction.
4.  **Visual Countdown**: After the object is removed, the system initiates a 5-second countdown visualized via a dynamic progress bar on the OLED.
5.  **Mechatronic Care (Controlled Closing)**: To preserve the gear integrity of the actuator, the closure is executed through a "Smooth Closing" algorithm (1° every 30ms).
6.  **Dual Warning Signaling**: During the closing phase, the system activates a synchronized alert consisting of a flashing Red LED and a graphical warning triangle on the display.

---

## Connection Table

| COMPONENT       | PIN (ESP32) | FUNCTION             |
|:----------------|:------------|:---------------------|
| OLED SDA        | GPIO 21     | Serial Data (I2C)    |
| OLED SCL        | GPIO 22     | Serial Clock (I2C)   |
| FC-51 Sensor    | GPIO 19     | Object Detection     |
| Servo PWM       | GPIO 18     | Lid Actuation        |
| Red LED         | GPIO 12     | Safety Alert         |
| Green LED       | GPIO 13     | Standby Indicator    |

---

This material is for classroom use only.
Commercial distribution is strictly prohibited.
Author: Iwan.