# 🚻 Smart Restroom Water Leakage & Occupancy Detection (ESP8266)

## 📌 Project Overview
This project is an IoT-based Smart Restroom Monitoring System built using ESP8266 (NodeMCU).

The system detects:
- 💧 Water leakage
- 🚶 Human presence (motion detection)

When water leakage is detected, the system checks whether a person is inside the restroom and sends a Push Notification using the Pushbullet API.

---

## 🛠️ Hardware Components

- ESP8266 (NodeMCU)
- PIR Motion Sensor
- Rain/Water Sensor Module
- Jumper Wires
- Power Supply

---

## 🔌 Pin Configuration

| Component      | ESP8266 Pin |
|---------------|------------|
| PIR Sensor    | D5         |
| Rain Sensor   | D6         |

---

## 🌐 How It Works

1. ESP8266 connects to WiFi.
2. Continuously monitors:
   - Rain sensor (Water leakage detection)
   - PIR sensor (Motion detection)
3. If water is detected:
   - If motion is detected → Sends alert (Person inside restroom)
   - If no motion → Sends critical leakage alert
4. Sends push notification via Pushbullet API over HTTPS.

---

## 📲 Push Notification Service

This project uses:
- Pushbullet REST API
- Secure HTTPS communication (`WiFiClientSecure`)
- JSON-based HTTP POST request

---

## 💻 Software Requirements

- Arduino IDE
- ESP8266 Board Package installed
- Required Libraries:
  - ESP8266WiFi.h
  - ESP8266HTTPClient.h
  - WiFiClientSecure.h

---

## 🚀 Installation Steps

1. Install Arduino IDE.
2. Add ESP8266 board manager URL:

   http://arduino.esp8266.com/stable/package_esp8266com_index.json

3. Go to:
   Tools → Board → Board Manager → Install ESP8266
4. Select Board:
   NodeMCU 1.0 (ESP-12E Module)
5. Update the following in the code:
   - WiFi SSID
   - WiFi Password
   - Pushbullet Access Token
6. Upload the code to ESP8266.
7. Open Serial Monitor (9600 baud rate).

---

## 🔐 Security Note

⚠️ Do NOT upload your real Pushbullet Access Token to GitHub.

Instead, use:

```cpp
const char* pushToken = "YOUR_PUSHBULLET_TOKEN";
