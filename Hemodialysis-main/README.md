
# 🩺 Hemodialysis System with Real-Time Monitoring and Safety Features

> 👨‍⚕️ **Medical Equipment Course – Task 04**  
> 🧠 *A real-world simulation of a Hemodialysis Machine using Arduino, Sensors, and Computer Vision*

---

## 📌 Overview

This project simulates a **functional hemodialysis system** built by our team as part of the Medical Equipment course. We focused on **patient safety, fluid flow control**, and **real-time alert systems** using embedded systems and basic computer vision.

Our system integrates:
- **Arduino-based sensing and control**
- **Real-time monitoring**
- **Emergency shutdown protocols**
- **Visual feedback**
- **Basic AI for posture analysis**

---

## ⚙️ System Features

### ✅ **1. Temperature Monitoring**
- Real-time sensing of dialysis fluid temperature.
- Ensures the fluid is within safe ranges for patient comfort and safety.
- Overheat triggers alert and halts the system.

### ✅ **2. Flow Rate Monitoring**
- Measures fluid movement across the dialyzer using flow sensors.
- Detects irregularities in circulation, which may indicate blockages or disconnections.

### ✅ **3. Blood Leakage Detection (LDR-based)**
- Light Dependent Resistor (LDR) detects abnormal presence of blood (based on light intensity changes).
- Activates buzzer and stops pump on detection of leakage.

### ✅ **4. LCD Display (I2C)**
- Displays live temperature, flow rate, and system status.
- Provides clinicians with clear and accessible feedback.

### ✅ **5. Emergency Stop**
- Triggered by any abnormal condition (temperature, flow, leakage).
- Ensures immediate shutdown of all operations for safety.

---

## 🪄 Bonus Feature: **Posture Monitoring Using Computer Vision**
- Uses a **custom Python + OpenCV algorithm** to track patient posture.
- Detects:
  - Arm bending
  - Arm movement
  - Leaning body posture
- Sends **alerts to medical staff** when risky movement patterns are detected during dialysis.

---

## 🧰 Technologies Used

| Component                | Description                               |
|--------------------------|-------------------------------------------|
| **Arduino UNO/Nano**     | Microcontroller for real-time control     |
| **Python + OpenCV**      | For posture monitoring and alerting       |
| **LDR Sensor**           | For blood leakage detection               |
| **Flow Sensor (YF-S201)**| For dialysate flow monitoring             |
| **Temperature Sensor (LM35/DS18B20)** | For fluid temp sensing       |
| **I2C LCD Display**      | For displaying parameters                 |
| **Buzzer**               | Audio alert system                        |
| **Push-button**          | Manual emergency stop                     |

---

## 🧪 System Architecture

```plaintext
[ Patient Arm ] 
    ↓ (LDR Sensor) — Detects blood
[ Flow Sensor ] → Arduino ← Temperature Sensor
                          ↓
                   [ I2C LCD Display ]
                          ↓
                     [ Buzzer + Stop ]
                          ↓
       Python + OpenCV — [ WebCam Input ]
             ↓
       Posture Alert to Clinicians
