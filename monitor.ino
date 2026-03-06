#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#define PIR_PIN    D5
#define RAIN_PIN   D6
#define MOTOR_PIN  D1
#define FLOW_PIN   D2 

const char* ssid = "abhi";
const char* password = "abhi2004";
const char* pushToken = "o.i5QgOmtIuDXjA5E9Owjfn43MFJ48Xc6Q";

unsigned long lastPush = 0;
const unsigned long cooldown = 3000;

unsigned long lastMotionTime = 0;
const unsigned long motionTimeout = 5000;  

bool motorState = false;

// ===== FLOW VARIABLES =====
volatile unsigned long pulseCount = 0;
unsigned long lastFlowCheck = 0;
float flowRate = 0;

// Interrupt for flow sensor
void IRAM_ATTR flowPulse() {
  pulseCount++;
}

void sendPush(String message) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, "https://api.pushbullet.com/v2/pushes")) {
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Access-Token", pushToken);

    String payload = "{";
    payload += "\"type\":\"note\",";
    payload += "\"title\":\"Washroom Alert 🚨\",";
    payload += "\"body\":\"" + message + "\"";
    payload += "}";

    https.POST(payload);
    https.end();
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(FLOW_PIN, INPUT_PULLUP); 

  digitalWrite(MOTOR_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flowPulse, FALLING); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  bool waterDetected = (digitalRead(RAIN_PIN) == LOW);
  bool motionDetected = (digitalRead(PIR_PIN) == HIGH);

  // ===============================
  // FLOW CALCULATION (Every 1 sec)
  // ===============================
  if (millis() - lastFlowCheck >= 1000) {

    noInterrupts();
    unsigned long pulses = pulseCount;
    pulseCount = 0;
    interrupts();

    flowRate = (pulses / 5880.0) * 60.0;  

    Serial.print("Flow Rate (L/min): ");
    Serial.println(flowRate);

    lastFlowCheck = millis();
  }

 
 if (motionDetected) {
  lastMotionTime = millis();

  if (!motorState) {
    digitalWrite(MOTOR_PIN, HIGH);
    motorState = true;
  }
}

if (motorState && millis() - lastMotionTime > motionTimeout) {
  digitalWrite(MOTOR_PIN, LOW);
  motorState = false;
}

  // 🔹 ORIGINAL RAIN SENSOR PUSH

  if (waterDetected && millis() - lastPush > cooldown) {

    if (motionDetected) {
      sendPush("All Good");
    } else {
      sendPush("Water is leaking and the person is NOT in the restroom ⚠️");
    }

    lastPush = millis();
  }

  // ===============================
  // ✅ SMART LEAK DETECTION (FLOW SENSOR)
  // ===============================
  
  if (!motionDetected && flowRate > 0.1 && millis() - lastPush > cooldown) {
    sendPush("Leak Detected! Water flowing without person ⚠️");
    lastPush = millis();
  }

  delay(50);
}
