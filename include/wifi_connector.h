#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <Arduino.h>

class WiFiConnector {
 public:
  WiFiConnector(const char* ssid, const char* password);
  
  void begin();
  bool isConnected();
  void update();

 private:
  const char* ssid;
  const char* password;
};

#endif // WIFI_CONNECTOR_H