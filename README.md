# RFID-Based Zone-Aware Speed Control System

## 🚗 Overview

This project implements a **zone-aware intelligent speed control system** for vehicles using RFID-based environmental detection.

The system automatically enforces speed restrictions when a vehicle enters predefined sensitive zones such as **schools, hospitals, or restricted areas**.

A working prototype was developed using an RC car integrated with an ESP8266 microcontroller, along with real-time monitoring via a web dashboard.

---

## ⚠️ Problem Statement

Speed limits in critical zones are often ignored due to:

* Lack of strict enforcement
* Human negligence
* Absence of automated control systems

This leads to increased risks, especially in high-sensitivity areas like school zones and hospitals.

---

## 💡 Solution

This system introduces **automatic speed enforcement** by:

* Detecting zones using RFID tags
* Dynamically restricting speed and gear
* Providing real-time telemetry to a monitoring dashboard

---

## 🏗️ System Architecture

```
Vehicle Unit (ESP8266)
│
├── RFID Reader (MFRC522)
├── Motor Driver (L298N)
├── Servo Motor (Steering)
│
├── Zone Detection Logic
├── Gear Restriction System
└── UDP Telemetry Transmission
        ↓
Python Server (UDP → WebSocket Bridge)
        ↓
Web Dashboard (Real-Time Monitoring)
```

---

## ✨ Features

* 📍 Zone-based speed limiting using RFID
* ⚙️ Automatic gear restriction (e.g., max 2 gears in restricted zones)
* 🔁 Reverse gear handling
* 📡 Real-time telemetry using UDP
* 🌐 Web dashboard via WebSockets
* 📱 Mobile control using RemoteXY

---

## ⚙️ Working Principle

1. Vehicle scans an RFID tag
2. Zone is identified (e.g., *Hospital Zone*)
3. Speed restriction is activated
4. Gear shifting is limited automatically
5. Telemetry data is sent via UDP
6. Dashboard updates in real time

---

## 🧰 Tech Stack

| Component       | Technology Used              |
| --------------- | ---------------------------- |
| Microcontroller | ESP8266                      |
| Communication   | WiFi (UDP Protocol)          |
| Backend         | Python (asyncio, websockets) |
| Dashboard       | HTML, JavaScript             |
| Mobile Control  | RemoteXY                     |
| Hardware        | MFRC522, L298N, Servo Motor  |

---

## 📁 Repository Structure

```
rfid-zone-speed-control/
│
├── firmware/
│   └── esp8266_controller.ino
│
├── server/
│   └── udp_ws_bridge.py
│
├── dashboard/
│   └── index.html
│
├── docs/
│   └── (images / demo videos)
│
├── requirements.txt
└── README.md
```

---

## ⚠️ Important Note

Speed values in this prototype are **simulated using PWM output**.

* No physical speed sensor or encoder is used
* Values represent *relative speed*, not actual vehicle speed

---

## ❌ Limitations

* No real speed feedback (no encoder)
* RFID requires physical tag infrastructure
* No fail-safe mechanism for communication loss
* Hardcoded network configuration (IP dependency)

---

## 🚀 Future Improvements

* Replace RFID with **GPS-based geofencing**
* Add **wheel encoder** for accurate speed measurement
* Implement **automatic braking system**
* Add **fail-safe logic for signal loss**
* Improve communication reliability (ACK/retry mechanism)
* Integrate with real automotive systems (CAN bus, etc.)

---

## 🎥 Demo

*(Add images, videos, or GIFs here)*

---

## 👨‍💻 Author

**Your Name**

---

## 📜 License

This project is for academic and demonstration purposes.
