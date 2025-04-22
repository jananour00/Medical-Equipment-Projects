# Infant Incubator System 👶  

![Image](https://github.com/user-attachments/assets/8f4ac897-c155-48fe-a589-9d6cf643dc75)
**Arduino-based infant incubator system that monitors and regulates temperature, humidity, and heart rate.**  
*(Developed for "Embedded Systems Project")*  

---  

## 📋 Table of Contents  
- [📝 Overview](#-overview)  
- [🌟 Features](#-features)  
- [🛠️ Components](#-components)  
- [🔧 Setup](#-setup)  
- [💎 Design](#-design)  
- [🎮 Usage](#-usage)  
- [📊 Project Goals](#-project-goals)  
- [👥 Team Roles & Memories](#-team-roles--memories)
- [📜 License](#-license)  

---  

## 📝 Overview  
This project implements an **infant incubator system** using an Arduino. It integrates a **temperature sensor**, **humidity sensor**, and **heart rate sensor** to monitor and regulate the incubator's environment. The system includes an **LCD display** for real-time data visualization, a **heating system** to maintain optimal temperature, and a **buzzer alarm** to alert if temperature drops below the desired level.  

---  

## 🌟 Features  
- **Real-time Monitoring**: Displays temperature, humidity, and heart rate.  
- **Automatic Temperature Control**: Adjusts heating elements as needed.  
- **LCD Display**: Shows live data for easy monitoring.  
- **Alarm System**: Triggers a buzzer when temperature is too low.  
- **User Interaction**: Allows manual reset of temperature settings.  

---  

## 🛠️ Components  
- Arduino board  
- Temperature sensor (DHT series)  
- Humidity sensor  
- Heart rate sensor  
- I2C LCD (16x2)  
- Heating element  
- Buzzer  
- Push button (for manual reset)  
- Power supply  
- Breadboard & jumper wires  

---  

## 🔧 Setup  
1. **Wire the Circuit**:  
   - **LCD**: `SDA → A4`, `SCL → A5`, `VCC → 5V`, `GND → GND`  
   - **Temperature & Humidity Sensor**: `Signal → D2`, `VCC → 5V`, `GND → GND`  
   - **Heart Rate Sensor**: `Signal → A0`, `VCC → 5V`, `GND → GND`  
   - **Heating Element**: Controlled via `PWM → D9`  
   - **Buzzer**: `Pin 8`  
   - **Push Button**: `Pin 10` (with pull-up resistor)  
   
2. **Upload Code**:  
   - Install `LiquidCrystal_I2C` and `DHT` libraries.  
   - Upload `sketch_mar25a.ino` to Arduino.  

3. **Adjust Settings**:  
   - Monitor live readings on the LCD.  
   - Reset temperature settings using the push button.  

---  
## Design

### Front
<img src="https://github.com/user-attachments/assets/8f4ac897-c155-48fe-a589-9d6cf643dc75" alt="Temperature Display" width="500" />  

### Top
<img src="https://github.com/user-attachments/assets/85249dfc-90ef-4fca-bb2a-6e12dfad9efe" alt="Temperature Display" width="500" />  

### R-side
<img src="https://github.com/user-attachments/assets/2ba06163-c562-4e6a-a97d-0a789efcacc1" alt="Temperature Display" width="500" />  

### L-side
<img src="https://github.com/user-attachments/assets/733f72c9-390c-4543-8018-774a12e77cbc" alt="Temperature Display" width="500" />  

### Circuit
<img src="https://github.com/user-attachments/assets/7934aca6-a758-42db-80f8-2bc39ac4a20f" alt="Temperature Display" width="500" />  

---  

## 🎮 Usage  
- Monitor **temperature, humidity, and heart rate** in real time.  
- The **buzzer alerts** when temperature drops below the set threshold.  
- The **heating element automatically adjusts** to maintain temperature.  
- Use the **push button** to manually reset the system.  

---  

## 📊 Project Goals  
This project aims to:  
✅ Provide continuous monitoring of infant health parameters.  
✅ Maintain optimal temperature and humidity inside the incubator.  
✅ Implement a safety alert system for abnormal conditions.  
✅ Allow user interaction for manual temperature adjustments.  

---  

## 👥 Team Roles  & Memories
| Member | Role | LinkedIn |  
|--------|------------------------------|----------------|  
| **Ibrahim Abdelkader** (Leader) | Hardware Setup & Integration | [LinkedIn](https://www.linkedin.com/in/ibrahim-abdelqader-93b9b124b/) |  
| **Mohamed Ehab** | Software Development & Testing | [LinkedIn](https://www.linkedin.com/in/mohamed-ehab-272394323/) |  
| **Menna Atef** | Project Design | [LinkedIn](https://www.linkedin.com/in/menna-atef-05355b32a/) |  
| **Nada Mostafa** | Documentation & Report Writing | [LinkedIn](https://www.linkedin.com/in/nada-mostafa-67853b324/) |  
| **Jana Nour** | Photographer & Presentation | [LinkedIn](https://www.linkedin.com/in/jana-nour-1510bb324/) |  

<img src="https://github.com/user-attachments/assets/22ef746e-db69-44b2-910a-221e0348073c" alt="Temperature Display" width="500" />  
<img src="https://github.com/user-attachments/assets/30e36c86-5256-4a70-839a-70ae97260164" alt="Temperature Display" width="500" />  
<img src="https://github.com/user-attachments/assets/e2cba238-ceec-4f41-8df1-9a0a8baa6ecf" alt="Temperature Display" width="500" />

---  

## 📜 License  
Open-source under MIT License. Contribute & innovate! 🚀
