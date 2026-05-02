# 🚗 Accident Detection System using ADXL335 + ESP32

## 📌 Project Overview
This project detects vehicle accidents using an **ADXL335 accelerometer** connected to an ESP32.  
The data is sent to ThingSpeak cloud and visualized using a real-time web dashboard.

---

## ⚙️ Components Used
- ESP32
- ADXL335 Accelerometer
- WiFi (IoT communication)
- ThingSpeak Cloud Platform

---

## 📊 Features
- 📡 Real-time sensor monitoring (AccX, AccY, AccZ)
- 📈 Live dashboard with graphs
- 🧠 Accident detection using:
  - Threshold logic
  - Machine Learning model
- 🗺️ Location tracking (Latitude & Longitude)
- 🚨 Alert system for accident detection

---

## 🌐 Live Dashboard
👉 (Add your GitHub Pages link here after deployment)

---

## 📂 Project Files
- `esp32_accident_detection.ino` → ESP32 code
- `accident_ml_model.m` → Machine learning model
- `feed.csv` → Sample data
- `index.html` → Dashboard UI

---

## 🔧 How it Works
1. ADXL335 captures acceleration values
2. ESP32 processes data and sends to ThingSpeak
3. Dashboard fetches data using API
4. System detects accident based on:
   - Sudden spike in acceleration
   - ML prediction

---

## ⚠️ Note
API keys are embedded for demo purposes. For production, use secure backend methods.

---

## 👨‍💻 Author
Srinidhi Sagar

---

## ⭐ If you like this project
Give it a star ⭐ on GitHub!
