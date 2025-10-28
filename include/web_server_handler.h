#ifndef WEB_SERVER_HANDLER_H
#define WEB_SERVER_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>

#ifdef ENABLE_WEB_SERVER
  #include <WebServer.h>
  // When web server is enabled, use WebServer pointer
  typedef WebServer* ServerPtr;
#else
  // When not enabled, use void pointer as placeholder
  typedef void* ServerPtr;
#endif

// The class is always declared with all methods, but behavior is conditional
class WebServerHandler {
 public:
  WebServerHandler(uint16_t port = 80);
  
  void begin();
  void update();
  void handleRoot();
  void handleStatus();
  void handleNotFound();
  
  // Methods to update component status
  void setEncoderPosition(int position);
  void setWiFiStatus(bool connected);
  void setOSCStatus(bool enabled);

 private:
  ServerPtr server;        // Pointer to WebServer or placeholder
  uint16_t port;
  
  // Component status - these are always available
  int lastEncoderPosition = 0;
  bool wifiConnected = false;
  bool oscEnabled = false;
};

#endif // WEB_SERVER_HANDLER_H