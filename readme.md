# 🚻 Smart Washroom Monitoring & Water Leak Detection System (ESP8266)

## 📌 Project Overview
This project is an **IoT-based Smart Washroom Monitoring System** built using the **ESP8266 (NodeMCU)**.  
The system monitors **washroom occupancy, detects water leakage, measures water flow, and automatically controls a motor or valve**.

By combining multiple sensors, the system can intelligently determine whether water usage is **normal or abnormal**. When suspicious conditions such as **water leakage without human presence** occur, the system sends **real-time alerts using the Pushbullet API**.

This solution helps reduce **water wastage, detect leaks early, and automate water usage in restrooms**.

---

# ⚙️ Features

- 🚶 Human presence detection using **PIR Motion Sensor**
- 💧 Water leakage detection using **Rain/Water Sensor**
- 🌊 Real-time water flow monitoring using **Flow Sensor**
- ⚡ Automatic **motor/valve control**
- 📲 **Push notifications** using Pushbullet API
- ⏱ Smart **motion timeout system**
- 🌐 WiFi-based IoT monitoring

---

# 🧠 System Working

## 1️⃣ WiFi Connection
When powered on, the ESP8266 connects to a WiFi network using the configured **SSID and password**.

Once connected, it can send **alerts through the internet** using Pushbullet.

---

## 2️⃣ Motion Detection (Occupancy Monitoring)

A **PIR motion sensor** connected to **D5** detects human movement inside the washroom.

- When motion is detected:
  - The system records the current time using `millis()`
  - The **motor or valve connected to D1 turns ON**

- When no motion is detected:
  - The system waits **5 seconds**
  - If no movement occurs, the **motor automatically turns OFF**

This prevents sudden shutdown caused by temporary PIR signal drops.

---

## 3️⃣ Water Leakage Detection

A **rain/water sensor** connected to **D6** detects water on the floor.

When water is detected, the system checks occupancy:

| Condition | System Action |
|-----------|--------------|
| Water + Motion detected | Sends **"All Good"** notification |
| Water detected + No motion | Sends **Leakage Warning Alert** |

---

## 4️⃣ Water Flow Monitoring

A **water flow sensor** connected to **D2** measures water usage.

The sensor produces pulses whenever water flows.

An **interrupt function** counts these pulses:

```cpp
void flowPulse() {
  pulseCount++;
}
