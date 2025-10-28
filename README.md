# ESP32 Rotary Encoder with WiFi, OSC, and Web Server

A versatile rotary encoder project for ESP32 that provides multiple interfaces for integration with other systems via WiFi, OSC (Open Sound Control), and a web server dashboard.

## Features

- KY-040 rotary encoder support with position tracking
- Push button functionality on encoder switch
- WiFi connectivity
- OSC (Open Sound Control) communication
- Web server dashboard for real-time monitoring
- Modular design with configurable components
- Compatible with Heltec WiFi LoRa 32 V3 board (easily adaptable to other ESP32 boards)

## Hardware Requirements

- ESP32 development board (tested with Heltec WiFi LoRa 32 V3)
- KY-040 rotary encoder module
- Breadboard and jumper wires for connections

## Pin Configuration

The default pin configuration is:

- CLK (Clock): GPIO 19
- DT (Data): GPIO 20
- SW (Switch): GPIO 21

> Note: You can modify these pins in `main.cpp` if needed for your specific board.

## Installation

1. Clone this repository or download the source code
2. Ensure you have PlatformIO installed in your development environment
3. Open the project in PlatformIO
4. Configure your WiFi credentials in `secrets.h` (create this file if it doesn't exist):
   ```cpp
   #ifndef SECRETS_H
   #define SECRETS_H
   
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
   
   #endif
   ```
5. Modify the OSC destination IP address in `main.cpp` to match your OSC receiver
6. Build and upload the project to your ESP32 board

## Configuration Options

The project includes a modular configuration system in `include/component_config.h`:

- `ENABLE_WIFI`: Enable/disable WiFi functionality
- `ENABLE_OSC`: Enable/disable OSC communication
- `ENABLE_WEB_SERVER`: Enable/disable web server dashboard
- Additional features can be easily enabled/disabled as needed

Comment out any of these defines to disable the corresponding functionality.

## Functionality

### Rotary Encoder
- Tracks rotation position with interrupt-based handling
- Sends position updates via configured communication methods
- Provides debounced button press detection

### OSC Communication
- Sends OSC messages on port 8000 by default
- `/encoder/position` - Float value of encoder position
- `/encoder/button` - String "pressed" when button is pressed

### Web Server
- Provides a web dashboard at the ESP32's IP address
- Shows real-time encoder position
- Displays WiFi and OSC status

## Usage

1. Upload the code to your ESP32 board
2. Connect the rotary encoder to the configured pins
3. Power on the ESP32 and wait for WiFi connection
4. Access the web dashboard using the ESP32's IP address
5. Use the rotary encoder to see position updates in real-time

## Customization

You can customize various aspects of the project:

1. **Communication Ports**: Modify OSC port (default 8000) and web server port (default 80) in `main.cpp`
2. **OSC Destination**: Update the destination IP address in `main.cpp` to send OSC messages to your desired receiver
3. **Component Enablement**: Modify `component_config.h` to enable/disable specific features
4. **Pin Assignments**: Change the CLK_PIN, DT_PIN, and SW_PIN definitions in `main.cpp`

## Troubleshooting

- If the encoder doesn't respond, verify the pin connections match the configuration
- Check WiFi credentials in `secrets.h` if connection fails
- Ensure your OSC receiver is configured to listen on the correct port and IP
- Monitor the serial output at 115200 baud rate for debugging information

## License

This project is open source and available under the MIT License.