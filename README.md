# Smart Train Ticket Verification System (SMTTVS)

---

## Overview

The Smart Train Ticket Verification System (SMTTVS) is an intelligent railway monitoring and ticket verification solution that assists Traveling Ticket Examiners (TTEs) in identifying unauthorized passengers, managing compartments, and improving train security through RFID and IoT technologies.

---

## Technology Stack

| Technology | Purpose |
|------------|----------|
| React.js | Frontend |
| Flask / Node.js | Backend |
| Firebase | Authentication & Database |
| ESP32 | Hardware Controller |
| RFID RC522 | Passenger Verification |
| PIR Sensors | Entry Monitoring |
| ESP32-CAM | Passenger Monitoring |
| Figma | UI Design |
| Vercel | Deployment |

---

## System Architecture

<p align="center">
  <img src="./assets/architecture.svg" width="900">
</p>

---

## Hardware Prototype

<table align="center">
<tr>
<td align="center">
<b>3D Printed Prototype</b><br><br>
<img src="./assets/prototype.jpg" width="400">
</td>

<td align="center">
<b>RFID Verification Unit</b><br><br>
<img src="./assets/rfid-unit.jpg" width="400">
</td>
</tr>
</table>

---

## 3D Printing Process

<p align="center">
  <img src="./assets/3d-print-1.jpg" width="450">
  <img src="./assets/3d-print-2.jpg" width="450">
</p>

---

## Real-Time Train Installation

<p align="center">
  <img src="./assets/train-installation.jpg" width="500">
</p>

---

## Mobile Application UI

### Splash Screen

<p align="center">
  <img src="./assets/splash.png" width="250">
</p>

---

### Registration | Login | OTP

<table align="center">
<tr>

<td align="center">
<img src="./assets/register.png" width="220"><br>
Registration
</td>

<td align="center">
<img src="./assets/login.png" width="220"><br>
Login
</td>

<td align="center">
<img src="./assets/otp.png" width="220"><br>
OTP Verification
</td>

</tr>
</table>

---

### Dashboard

<p align="center">
  <img src="./assets/dashboard.png" width="250">
</p>

---

### Take Charge Module

<p align="center">
  <img src="./assets/take-charge.png" width="250">
</p>

---

### Compartment Details

<p align="center">
  <img src="./assets/compartment.png" width="250">
</p>

---

## Seat Management System

<table align="center">

<tr>

<td align="center">
<img src="./assets/general.png" width="220"><br>
General
</td>

<td align="center">
<img src="./assets/sleeper.png" width="220"><br>
Sleeper
</td>

<td align="center">
<img src="./assets/ac3.png" width="220"><br>
AC 3-Tier
</td>

</tr>

<tr>

<td align="center">
<img src="./assets/ac2.png" width="220"><br>
AC 2-Tier
</td>

<td align="center">
<img src="./assets/ac1.png" width="220"><br>
AC 1-Tier
</td>

<td align="center">
<img src="./assets/chaircar.png" width="220"><br>
Chair Car
</td>

</tr>

</table>

---

## Unauthorized Passenger Detection

<p align="center">
  <img src="./assets/alert.png" width="250">
</p>

---

## Train Modification Module

<p align="center">
  <img src="./assets/modification.png" width="250">
</p>

---

## System Workflow

```text
RFID Scan
    │
    ▼
Passenger Authentication
    │
    ▼
Entry Detection
    │
    ▼
Compartment Monitoring
    │
    ▼
Unauthorized Entry Detection
    │
    ▼
Alert Generation
    │
    ▼
TTE Verification
    │
    ▼
Fine Processing
```

---

## Key Features

- RFID Based Passenger Verification
- Unauthorized Passenger Detection
- Real-Time TTE Alerts
- Smart Seat Management
- OTP Based Authentication
- Firebase Integration
- Train Modification Module
- Fine Charging System
- Mobile Friendly Interface
- IoT Enabled Monitoring

---

## Developer

**Siddharth B**

Aspiring Software Engineer  
IoT Developer  
Full Stack Developer

---

## License

This project is developed for educational, research, and innovation purposes.
