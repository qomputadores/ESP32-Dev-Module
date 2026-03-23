# Project 08: Smart Trash Can with Serial Monitoring

## 1. Description
This project demonstrates an advanced mechatronic application using the ESP32. It features an automated lid system that uses an infrared sensor for detection and a servomotor for actuation. The code implements **Blocking Loops** for user safety and **Smooth Movement Algorithms** to protect the internal gears of the servomotor. The wiring is optimized for the **30-pin ESP32**, keeping all connections on the right side of the board.

## 2. Hardware Anatomy
* **ESP32 PWM Management:** Uses the `ESP32Servo` library to handle high-resolution PWM timers for precise motor positioning.
* **FC-51 IR Sensor:** Detects proximity through infrared reflection. It uses **Active LOW** logic, returning 0 when an obstacle is present.
* **Mechanical Protection:** The lid uses a fast opening profile (90°) and a software-controlled slow closing profile (for loop) to reduce mechanical stress.

## 3. Buzzer Integration (Optional)
* **Buzzer Support:** A Buzzer Module can be connected to any free GPIO (e.g., GPIO 21) to provide acoustic feedback during the opening or countdown phases.

## 4. Power and Safety
* **External Power (VIN):** Servomotors can cause voltage drops. Connect the Servo's VCC to the **VIN pin (5V)** instead of the 3.3V pin to ensure stability.
* **Current Management:** Always verify the GND connection between the ESP32 and the Servo to avoid erratic movements.
* **Side-Wiring Logic:** All pins (18, 19, 12, 13) are located on the same side to keep the prototype organized.

## 5. Connection Table (Single-Side Layout)

| Component | ESP32 GPIO | Side Label | Function |
| :--- | :--- | :--- | :--- |
| Servo Signal | 18 | D18 | PWM Output |
| FC-51 Out | 19 | D19 | Digital In |
| LED Red | 12 | D12 | Status (Open) |
| LED Green | 13 | D13 | Status (Ready) |
| Servo VCC | VIN | 5V | Power |

---
**This material is for classroom use only. Commercial distribution is strictly prohibited.**

**Author: Iwan**