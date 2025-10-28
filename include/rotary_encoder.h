#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

class RotaryEncoder {
 public:
  RotaryEncoder(uint8_t clk, uint8_t dt);

  void begin();
  void update();
  int getPosition() const;

  void handleInterrupt();
  
  // Callback for position changes
  void setOnChangeCallback(void (*callback)(int pos));

  static RotaryEncoder* instance;

 private:
  uint8_t clkPin, dtPin;
  volatile int pos;
  volatile int delta;
  volatile int lastCLK;
  void (*onChangeCallback)(int pos) = nullptr;
};

class EncoderButton {
 public:
  EncoderButton(uint8_t pin, unsigned long debounceMs = 50);

  void begin();
  void update();

  // Callback for button press
  void setOnPressCallback(void (*callback)());
  virtual void onPress();

 private:
  void (*onPressCallback)() = nullptr;
  uint8_t pin;
  bool lastState, pressed;
  unsigned long debounceMs, lastChange;
};

void IRAM_ATTR encoderISR();

#endif // ROTARY_ENCODER_H