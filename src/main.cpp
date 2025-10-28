#include <Arduino.h>

#include "component_config.h"
#include "secrets.h"  // Include secrets file
#include "rotary_encoder.h"

#ifdef ENABLE_WIFI
#include "wifi_connector.h"
WiFiConnector wifi(WIFI_SSID, WIFI_PASSWORD);
#endif

#ifdef ENABLE_OSC
#include "osc_handler.h"
OSCHandler osc(8000);  // OSC on port 8000 by default
#endif

#ifdef ENABLE_WEB_SERVER
#include "web_server_handler.h"
WebServerHandler webServer(80);  // Web server on port 80
#endif

#define CLK_PIN 19
#define DT_PIN 20
#define SW_PIN 21

RotaryEncoder encoder(CLK_PIN, DT_PIN);
EncoderButton button(SW_PIN);

// Function to send OSC message when encoder position changes
void onEncoderChange(int pos) {
#ifdef ENABLE_OSC
  osc.sendFloat("/encoder/position", (float)pos);
#endif

#ifdef ENABLE_WEB_SERVER
  webServer.setEncoderPosition(pos);
#endif
}

// Function to send OSC message when encoder button is pressed
void onEncoderButtonPress() {
#ifdef ENABLE_OSC
  osc.sendString("/encoder/button", "pressed");
#endif
}

void setup() {
  Serial.begin(115200);
#ifdef ENABLE_WIFI
  #ifdef ENABLE_OSC
    #ifdef ENABLE_WEB_SERVER
    Serial.println("KY-040 Rotary Encoder with WiFi, OSC, and Web Server");
    #else
    Serial.println("KY-040 Rotary Encoder with WiFi and OSC");
    #endif
  #else
    #ifdef ENABLE_WEB_SERVER
    Serial.println("KY-040 Rotary Encoder with WiFi and Web Server");
    #else
    Serial.println("KY-040 Rotary Encoder with WiFi");
    #endif
  #endif
#else
  #ifdef ENABLE_OSC
    #ifdef ENABLE_WEB_SERVER
    Serial.println("KY-040 Rotary Encoder with OSC and Web Server");
    #else
    Serial.println("KY-040 Rotary Encoder with OSC");
    #endif
  #else
    #ifdef ENABLE_WEB_SERVER
    Serial.println("KY-040 Rotary Encoder with Web Server");
    #else
    Serial.println("KY-040 Rotary Encoder");
    #endif
  #endif
#endif

  encoder.begin();
  button.begin();

// Set callbacks for OSC messages when components are enabled
#ifdef ENABLE_OSC
  encoder.setOnChangeCallback(onEncoderChange);
  button.setOnPressCallback(onEncoderButtonPress);
#endif

#ifdef ENABLE_WIFI
  wifi.begin();
#endif

#ifdef ENABLE_OSC
  // Set destination IP for OSC messages (change as needed)
  uint8_t destIP[4] = {192, 168, 1, 100};  // Example IP - change to your OSC receiver
  osc.setDestinationIP(destIP, 8000);
  osc.begin();
#endif

#ifdef ENABLE_WEB_SERVER
  // Initialize web server after WiFi is set up
  webServer.begin();
#endif
}

void loop() {
  encoder.update();
  button.update();

#ifdef ENABLE_WIFI
  wifi.update();
  
  // Update web server with WiFi status
  #ifdef ENABLE_WEB_SERVER
  webServer.setWiFiStatus(wifi.isConnected());
  #endif
#endif

#ifdef ENABLE_OSC
  osc.update();
  
  // Update web server with OSC status
  #ifdef ENABLE_WEB_SERVER
  webServer.setOSCStatus(true);
  #endif
#endif

#ifdef ENABLE_WEB_SERVER
  webServer.update();
#endif
}
