#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#define PIR_PIN   D5
#define RAIN_PIN  D6

const char* ssid = "desktop";
const char* password = "11111111";
const char* pushToken = "o.YfReWQyhLBlWSwes0zSHhU5eEEEANh2T";

unsigned long lastPush = 0;
const unsigned long cooldown = 1000; 

void sendPush(String message) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, "https://api.pushbullet.com/v2/pushes")) {
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Access-Token", pushToken);

    String payload = "{";
    payload += "\"type\":\"note\",";
    payload += "\"title\":\"_\","; // change _ to the expecting output 
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

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  bool waterDetected = (digitalRead(RAIN_PIN) == LOW); // LOW = water
  bool motionDetected = (digitalRead(PIR_PIN) == HIGH);

  if (waterDetected && millis() - lastPush > cooldown) {

    if (motionDetected) {
      sendPush(""); // print the msg if the person is inside the restroom
    } else {
      sendPush("Water is leaking and the person is NOT in the restroom ⚠️");
    }

    lastPush = millis();
  }
}
