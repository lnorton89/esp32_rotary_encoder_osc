#include "wifi_connector.h"

#include <Arduino.h>
#include <WiFi.h>

WiFiConnector::WiFiConnector(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

void WiFiConnector::begin() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  const int maxAttempts = 20;  // 20 * 500ms = 10 seconds timeout

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi");
  }
}

bool WiFiConnector::isConnected() { return WiFi.status() == WL_CONNECTED; }

void WiFiConnector::update() {
  // Add any periodic WiFi maintenance here if needed
  // For now, just ensure we're still connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.reconnect();
  }
}