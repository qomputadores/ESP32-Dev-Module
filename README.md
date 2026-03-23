# ESP32 Dev Module - Advanced Systems and IoT

## 1. Repository Context
This repository is dedicated to the development of projects using the **ESP32 Dev Module**. It documents the technical transition from basic microcontrollers to high-performance embedded systems, exploring dual-core processing and integrated wireless connectivity.

## 2. Software Configuration (Arduino IDE)

To program the ESP32, the development environment must be correctly configured:

1. **IDE Download:** Download and install the latest version of the **Arduino IDE** from the official site (arduino.cc).
2. **Boards Manager:**
   * Go to `File` > `Preferences`.
   * In the "Additional Boards Manager URLs" field, paste the following link:
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   * Go to `Tools` > `Board` > `Boards Manager`, search for **esp32** and install it.
3. **Board Selection:**
   * In the boards dropdown menu, first select the **Espressif** category.
   * Then, search and select: **ESP32 Dev Module**.
   * Ensure you select the correct **COM Port** recognized by your computer.

## 3. Upload Protocol
On some ESP32 models, the auto-reset system may fail. If the Arduino IDE console displays the message **"Connecting........_____"** and does not proceed:
* Press and hold the **BOOT** (or EN) button on your ESP32 board.
* Release it as soon as you see the program writing process begin (% progress).

## 4. Hardware Specifications
* **Processor:** Xtensa Dual-Core 32-bit LX6.
* **Operating Voltage:** 3.3V (Pins are **NOT** 5V tolerant).
* **Connectivity:** Integrated WiFi and Bluetooth.



## 5. Development Standard
* **Nomenclature:** Strict use of **Upper CamelCase** for variables and functions.
* **Documentation:** Line-by-line comments in **English**.
* **Pin Management:** Use of the `#define` preprocessor directive.

## 6. Project Index
* **01_Blink_Internal:** Initial test of architecture and program uploading.
* *(Projects will be indexed automatically as they are synchronized).*

---
**This material is for classroom use only. Commercial distribution is strictly prohibited.**

**Author: Iwan**
