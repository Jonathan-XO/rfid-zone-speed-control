# 🚗 RFID-Based Zone-Aware Speed Control System

## 📌 Overview

This project implements a **zone-aware intelligent speed limiting system** for vehicles using RFID-based environmental detection.

When a vehicle enters restricted areas such as **schools or hospitals**, the system automatically enforces speed limits by restricting gear control.

A working prototype was built using an RC car integrated with an **ESP8266**, RFID module, and a real-time monitoring dashboard.

---

## ⚠️ Problem Statement

Drivers frequently ignore speed limits in sensitive zones like schools and hospitals, creating serious safety risks.

Manual enforcement methods are inconsistent and unreliable.

---

## 💡 Solution

This system enforces **automatic speed restriction** by:

* Detecting zones using RFID tags
* Dynamically limiting vehicle speed
* Restricting gear shifting in real-time

---

## 🏗️ System Architecture

```
[ESP8266 Vehicle Unit]
   ├── RFID Zone Detection
   ├── Gear Restriction Logic
   └── UDP Telemetry Transmission
                ↓
[Python Server]
   └── UDP → WebSocket Bridge
                ↓
[Web Dashboard]
   └── Real-Time Monitoring (Speed, Gear, Zone)
```

---

## ✨ Features

* RFID-based zone detection
* Automatic speed limiting
* Gear restriction (max 2 gears in restricted zones)
* Reverse gear handling
* Real-time telemetry using UDP
* Live dashboard via WebSockets
* Mobile control using RemoteXY

---

## ⚙️ Working Principle

1. RFID tag is scanned by the vehicle
2. Zone is identified (e.g., School Zone)
3. Speed restriction is enforced
4. Gear shifting is limited automatically
5. Data is transmitted via UDP
6. Dashboard updates in real-time

---

## 🛠️ Tech Stack

**Hardware**

* ESP8266
* MFRC522 RFID Module
* L298N Motor Driver
* Servo Motor

**Software**

* Python (asyncio, websockets)
* cpp (Arduino ide)
* RemoteXY (Mobile UI)
* HTML/CSS/JS Dashboard

---

## 📉 Limitations

* No real speed feedback (no encoder used)
* RFID requires physical tags (not scalable)
* No fail-safe mechanism on communication loss

---

## 🚀 Future Improvements

* GPS-based geofencing instead of RFID
* Wheel encoder for real speed measurement
* Automatic braking system
* Reliable communication (ACK/retry mechanisms)
* Integration with real vehicle ECU systems

---

## ⚠️ Important Note

Speed values in this prototype are **estimated based on PWM output**, not actual measured velocity.

---

## 👨‍💻 Author

Jonathan,kishore R
