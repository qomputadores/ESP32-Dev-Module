# Project 07: Smart Night Light with Serial Feedback

## 1. Description
This project focuses on energy efficiency using **Dual-Condition Logic**. The system monitors ambient light levels and physical presence to control a 5-LED array. The light only activates when the environment is dark **AND** an object is detected. The wiring is optimized for the **30-pin ESP32**, using pins located on the right side to simplify breadboard management.

## 2. Hardware Anatomy
* **LDR (Photoresistor):** Monitors light intensity. The ESP32 12-bit ADC converts this into values from 0 to 4095.
* **Infrared Sensor (FC-51):** Detects proximity using infrared reflection. It operates on **Active LOW** logic (outputs 0 when triggered).
* **Boolean Logic:** Uses the **AND (&&)** operator to evaluate two logical states before taking action.

## 3. Buzzer Integration (Component vs. Module)
* **Active Buzzer Component:** 2-pin part. Requires a resistor and direct GPIO connection.
* **Buzzer Module:** 3-pin PCB (VCC, GND, Signal). Easier to connect and includes internal driving circuitry.

## 4. Power and Safety
* **3.3V Logic:** The ESP32 GPIO pins are **NOT** 5V tolerant. Always power sensors with 3.3V when possible.
* **Resistors:** Each LED must have its own **220 Ohm resistor** to prevent overcurrent damage.
* **Common Ground:** Ensure all components share the same GND rail on the breadboard.

## 5. Connection Table (Single-Side Layout)

| Component | ESP32 GPIO | Side Label | Function |
| :--- | :--- | :--- | :--- |
| LDR Out | 32 | D32 | Analog In |
| FC-51 Out | 33 | D33 | Digital In |
| LED 1 | 25 | D25 | Output |
| LED 2 | 26 | D26 | Output |
| LED 3 | 27 | D27 | Output |
| LED 4 | 14 | D14 | Output |
| LED 5 | 12 | D12 | Output |

---
**This material is for classroom use only. Commercial distribution is strictly prohibited.**

**Author: Iwan**