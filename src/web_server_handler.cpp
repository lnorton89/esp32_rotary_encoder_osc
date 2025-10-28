#include "component_config.h"  // Include config first to define macros
#include "web_server_handler.h"

#ifdef ENABLE_WEB_SERVER

#include <WebServer.h>

WebServerHandler::WebServerHandler(uint16_t port) : port(port) {
  // Cast the void* to WebServer* and initialize
  server = new WebServer(port);
}

// Define generateHTMLInternal as a static function in the cpp file
static String generateHTMLInternal(int lastEncoderPosition, bool wifiConnected, bool oscEnabled) {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Rotary Encoder Status</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; background-color: #f0f0f0; }";
  html += ".container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += "h1 { color: #333; text-align: center; }";
  html += ".status-box { background: #f8f8f8; border: 1px solid #ddd; border-radius: 5px; padding: 15px; margin: 10px 0; }";
  html += ".status-item { display: flex; justify-content: space-between; padding: 5px 0; }";
  html += ".status-label { font-weight: bold; }";
  html += ".status-value { color: #555; }";
  html += ".connected { color: green; }";
  html += ".disconnected { color: red; }";
  html += ".enabled { color: green; }";
  html += ".disabled { color: orange; }";
  html += "button { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; margin: 5px; }";
  html += "button:hover { background-color: #45a049; }";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1>Rotary Encoder Status Dashboard</h1>";
  
  html += "<div class=\"status-box\">";
  html += "<h2>Component Status</h2>";
  
  html += "<div class=\"status-item\">";
  html += "<span class=\"status-label\">Encoder Position:</span>";
  html += "<span class=\"status-value\" id=\"encoderPos\">" + String(lastEncoderPosition) + "</span>";
  html += "</div>";
  
  html += "<div class=\"status-item\">";
  html += "<span class=\"status-label\">WiFi Status:</span>";
  html += "<span class=\"status-value " + String(wifiConnected ? "connected" : "disconnected") + "\" id=\"wifiStatus\">";
  html += wifiConnected ? "Connected" : "Disconnected";
  html += "</span>";
  html += "</div>";
  
  html += "<div class=\"status-item\">";
  html += "<span class=\"status-label\">OSC Enabled:</span>";
  html += "<span class=\"status-value " + String(oscEnabled ? "enabled" : "disabled") + "\" id=\"oscStatus\">";
  html += oscEnabled ? "Yes" : "No";
  html += "</span>";
  html += "</div>";
  
  html += "<div class=\"status-item\">";
  html += "<span class=\"status-label\">IP Address:</span>";
  html += "<span class=\"status-value\" id=\"ipAddress\">" + WiFi.localIP().toString() + "</span>";
  html += "</div>";
  
  html += "</div>";
  
  html += "<button onclick=\"updateStatus()\">Refresh Status</button>";
  html += "<button onclick=\"location.reload()\">Reload Page</button>";
  
  html += "<script>";
  html += "function updateStatus() {";
  html += "  fetch('/status')";
  html += "    .then(response => response.json())";
  html += "    .then(data => {";
  html += "      document.getElementById('encoderPos').textContent = data.encoder_position;";
  html += "      document.getElementById('wifiStatus').textContent = data.wifi_connected === true ? 'Connected' : 'Disconnected';";
  html += "      document.getElementById('wifiStatus').className = data.wifi_connected === true ? 'status-value connected' : 'status-value disconnected'; "; 
  html += "      document.getElementById('oscStatus').textContent = data.osc_enabled === true ? 'Yes' : 'No';";
  html += "      document.getElementById('oscStatus').className = data.osc_enabled === true ? 'status-value enabled' : 'status-value disabled';"; 
  html += "      document.getElementById('ipAddress').textContent = data.ip_address;";
  html += "    })";
  html += "    .catch(error => console.error('Error:', error));";
  html += "}";
  html += "setInterval(updateStatus, 2000);";  // Update every 2 seconds
  html += "</script>";
  
  html += "</div></body></html>";
  
  return html;
}

void WebServerHandler::begin() {
  WebServer* webServer = static_cast<WebServer*>(server);
  if (!webServer) return;
  
  // Define request handlers - capture member variables by reference to the object
  auto self = this; // capture this for use in lambdas
  webServer->on("/", HTTP_GET, [self, webServer]() {
    String html = generateHTMLInternal(self->lastEncoderPosition, self->wifiConnected, self->oscEnabled);
    webServer->send(200, "text/html", html);
  });
  
  webServer->on("/status", HTTP_GET, [self, webServer]() {
    String statusJSON = "{";
    statusJSON += "\"encoder_position\":" + String(self->lastEncoderPosition) + ",";
    statusJSON += "\"wifi_connected\":" + String(self->wifiConnected ? "true" : "false") + ",";
    statusJSON += "\"osc_enabled\":" + String(self->oscEnabled ? "true" : "false") + ",";
    statusJSON += "\"wifi_status\":\"" + String(WiFi.status() == WL_CONNECTED ? "connected" : "disconnected") + "\",";
    statusJSON += "\"ip_address\":\"" + WiFi.localIP().toString() + "\"";
    statusJSON += "}";
    
    webServer->send(200, "application/json", statusJSON);
  });
  
  webServer->onNotFound([webServer]() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webServer->uri();
    message += "\nMethod: ";
    message += (webServer->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += webServer->args();
    message += "\n";
    for (uint8_t i = 0; i < webServer->args(); i++) {
      message += " " + webServer->argName(i) + ": " + webServer->arg(i) + "\n";
    }
    webServer->send(404, "text/plain", message);
  });
  
  webServer->begin();
  Serial.println("Web server started on port " + String(port));
}

void WebServerHandler::update() {
  WebServer* webServer = static_cast<WebServer*>(server);
  if (webServer) {
    webServer->handleClient();  // Handle incoming client requests
  }
}

void WebServerHandler::handleRoot() {
  // This won't be called directly; handlers are set up in begin()
}

void WebServerHandler::handleStatus() {
  // This won't be called directly; handlers are set up in begin()
}

void WebServerHandler::handleNotFound() {
  // This won't be called directly; handlers are set up in begin()
}

void WebServerHandler::setEncoderPosition(int position) {
  lastEncoderPosition = position;
}

void WebServerHandler::setWiFiStatus(bool connected) {
  wifiConnected = connected;
}

void WebServerHandler::setOSCStatus(bool enabled) {
  oscEnabled = enabled;
}

#else  // ENABLE_WEB_SERVER is not defined, provide stub implementations

// Provide stub implementations when web server is disabled
WebServerHandler::WebServerHandler(uint16_t port) : port(port) {
  // Initialize server as nullptr (void pointer can be assigned nullptr)
  server = nullptr;
}

void WebServerHandler::begin() {
  // Do nothing
}

void WebServerHandler::update() {
  // Do nothing
}

void WebServerHandler::handleRoot() {
  // Do nothing
}

void WebServerHandler::handleStatus() {
  // Do nothing
}

void WebServerHandler::handleNotFound() {
  // Do nothing
}

void WebServerHandler::setEncoderPosition(int position) {
  lastEncoderPosition = position;
}

void WebServerHandler::setWiFiStatus(bool connected) {
  wifiConnected = connected;
}

void WebServerHandler::setOSCStatus(bool enabled) {
  oscEnabled = enabled;
}

#endif // ENABLE_WEB_SERVER