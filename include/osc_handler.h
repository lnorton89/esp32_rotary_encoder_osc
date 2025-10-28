#ifndef OSC_HANDLER_H
#define OSC_HANDLER_H

#include <Arduino.h>
#include <WiFiUdp.h>

// The class is always declared, but behavior is conditional based on ENABLE_OSC
class OSCHandler {
 public:
  OSCHandler(uint16_t port = 8000);
  ~OSCHandler();

  void begin();
  void update();
  bool sendFloat(const char* address, float value);
  bool sendInt(const char* address, int value);
  bool sendString(const char* address, const char* value);
  void setDestinationIP(const uint8_t* ip, uint16_t port);

 private:
  WiFiUDP udp;
  uint16_t port;
  uint8_t destinationIP[4];
  uint16_t destinationPort;
};

#endif // OSC_HANDLER_H