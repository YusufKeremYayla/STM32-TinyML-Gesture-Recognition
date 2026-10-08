# 🚀 STM32 TinyML Gesture Recognition & Anomaly Detection

## 📌 Overview
This project implements an edge AI (TinyML) gesture recognition system on an STM32 microcontroller using the **Edge Impulse** platform. It utilizes a **LIS3DSH 3-axis accelerometer** to classify 4 specific movements and features a **K-Means Anomaly Detection** filter to actively reject unknown or untrained gestures.

## ✨ Key Features
* **Motion Classification:** Accurately recognizes `Circle`, `Left-Right`, `Up-Down`, and `Idle` states.
* **Anomaly Rejection (Security Filter):** Utilizes K-Means clustering to calculate an anomaly score. If an unrecognized gesture is performed, the system safely rejects it rather than forcing a false Softmax prediction.
* **Non-Blocking Architecture:** Uses a custom Circular Buffer to continuously collect 3-axis sensor data without halting the main CPU operations.
* **Modular C/C++ Design:** Clean separation of concerns between hardware control (`main.cpp`) and the AI inference logic (`ai_controller.cpp`) using `extern "C"` linkage.

## 🛠️ Hardware & Software
* **Microcontroller:** STM32F4 Series (Tested on STM32F407)
* **Sensor:** LIS3DSH Accelerometer (SPI Interface)
* **Development Tools:** STM32CubeIDE, STM32CubeMX
* **Machine Learning:** Edge Impulse SDK (C++ Deployment)

## 🚀 Getting Started
1. Clone this repository to your local machine.
2. Open the project directory using **STM32CubeIDE**.
3. Build the project (`Project -> Build All`).
4. Flash the compiled firmware to your STM32 development board.
5. Open a Serial Terminal (Baud Rate: `115200`) to view real-time inference results, Softmax probabilities, and Anomaly scores.

## 📁 Core Project Structure
* `Core/Src/ai_controller.cpp`: Handles Edge Impulse DSP conversions, inference execution, and anomaly threshold checking.
* `Core/Src/main.cpp`: Manages hardware initialization, sensor reading via EXTI interrupts, and UART logging.
* `Core/Inc/circular_buffer.h`: Queue data structure for efficient, non-blocking sensor data accumulation.

---
*Designed for industrial-grade gesture filtering at the edge.*
