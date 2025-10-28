#include "osc_handler.h"

#include <WiFi.h>
#include <WiFiUdp.h>

#include "component_config.h"  // Include the config to access the ENABLE_OSC macro

#ifdef ENABLE_OSC

OSCHandler::OSCHandler(uint16_t port) : port(port), destinationPort(8000) {
  // Default destination IP (localhost)
  destinationIP[0] = 127;
  destinationIP[1] = 0;
  destinationIP[2] = 0;
  destinationIP[3] = 1;
}

OSCHandler::~OSCHandler() { udp.stop(); }

void OSCHandler::begin() {
  udp.begin(port);
  Serial.printf("OSC Server listening on port %d\n", port);
}

void OSCHandler::update() {
  // Check for incoming OSC messages if needed
  // For now, just a placeholder for future implementation
}

bool OSCHandler::sendFloat(const char* address, float value) {
#ifdef ENABLE_WIFI
  if (WiFi.status() != WL_CONNECTED)
    return false;  // Only send if connected to WiFi
#endif

  udp.beginPacket(destinationIP, destinationPort);

  // Write OSC address
  udp.write((uint8_t*)address, strlen(address) + 1);

  // Pad address to multiple of 4
  int addrLen = strlen(address) + 1;
  while (addrLen % 4 != 0) {
    udp.write((uint8_t)0);
    addrLen++;
  }

  // Write type tag
  udp.write((uint8_t*)",f", 2);
  udp.write((uint8_t)0);  // null terminator
  udp.write((uint8_t)0);  // padding

  // Write float value in big-endian format
  uint32_t floatBits;
  memcpy(&floatBits, &value, 4);

  // Convert to big-endian
  uint8_t floatBytes[4];
  floatBytes[0] = (floatBits >> 24) & 0xFF;
  floatBytes[1] = (floatBits >> 16) & 0xFF;
  floatBytes[2] = (floatBits >> 8) & 0xFF;
  floatBytes[3] = floatBits & 0xFF;

  udp.write(floatBytes, 4);

  bool result = udp.endPacket();
  return result;
}

bool OSCHandler::sendInt(const char* address, int value) {
#ifdef ENABLE_WIFI
  if (WiFi.status() != WL_CONNECTED)
    return false;  // Only send if connected to WiFi
#endif

  udp.beginPacket(destinationIP, destinationPort);

  // Write OSC address
  udp.write((uint8_t*)address, strlen(address) + 1);

  // Pad address to multiple of 4
  int addrLen = strlen(address) + 1;
  while (addrLen % 4 != 0) {
    udp.write((uint8_t)0);
    addrLen++;
  }

  // Write type tag
  udp.write((uint8_t*)",i", 2);
  udp.write((uint8_t)0);  // null terminator
  udp.write((uint8_t)0);  // padding

  // Write int value in big-endian format
  uint8_t intBytes[4];
  intBytes[0] = (value >> 24) & 0xFF;
  intBytes[1] = (value >> 16) & 0xFF;
  intBytes[2] = (value >> 8) & 0xFF;
  intBytes[3] = value & 0xFF;

  udp.write(intBytes, 4);

  bool result = udp.endPacket();
  return result;
}

bool OSCHandler::sendString(const char* address, const char* value) {
#ifdef ENABLE_WIFI
  if (WiFi.status() != WL_CONNECTED)
    return false;  // Only send if connected to WiFi
#endif

  udp.beginPacket(destinationIP, destinationPort);

  // Write OSC address
  udp.write((uint8_t*)address, strlen(address) + 1);

  // Pad address to multiple of 4
  int addrLen = strlen(address) + 1;
  while (addrLen % 4 != 0) {
    udp.write((uint8_t)0);
    addrLen++;
  }

  // Write type tag
  udp.write((uint8_t*)",s", 2);
  udp.write((uint8_t)0);  // null terminator
  udp.write((uint8_t)0);  // padding

  // Write string value
  udp.write((uint8_t*)value, strlen(value) + 1);

  // Pad string to multiple of 4
  int strLen = strlen(value) + 1;
  while (strLen % 4 != 0) {
    udp.write((uint8_t)0);
    strLen++;
  }

  bool result = udp.endPacket();
  return result;
}

void OSCHandler::setDestinationIP(const uint8_t* ip, uint16_t port) {
  for (int i = 0; i < 4; i++) {
    destinationIP[i] = ip[i];
  }
  destinationPort = port;
}

#else  // ENABLE_OSC is not defined, provide stub implementations

// Provide stub implementations when OSC is disabled
OSCHandler::OSCHandler(uint16_t port) : port(port), destinationPort(8000) {
  // Initialize with default values but do nothing else
}

OSCHandler::~OSCHandler() {
  // Do nothing
}

void OSCHandler::begin() {
  // Do nothing
}

void OSCHandler::update() {
  // Do nothing
}

bool OSCHandler::sendFloat(const char* address, float value) {
  // Do nothing, return false to indicate failure
  return false;
}

bool OSCHandler::sendInt(const char* address, int value) {
  // Do nothing, return false to indicate failure
  return false;
}

bool OSCHandler::sendString(const char* address, const char* value) {
  // Do nothing, return false to indicate failure
  return false;
}

void OSCHandler::setDestinationIP(const uint8_t* ip, uint16_t port) {
  // Do nothing
}

#endif  // ENABLE_OSC