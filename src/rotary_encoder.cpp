#include "rotary_encoder.h"

#include <Arduino.h>

// ==========================
// RotaryEncoder Implementation
// ==========================

RotaryEncoder::RotaryEncoder(uint8_t clk, uint8_t dt)
    : clkPin(clk), dtPin(dt), lastCLK(HIGH), pos(0), delta(0) {}

void RotaryEncoder::begin() {
  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
  lastCLK = digitalRead(clkPin);
  instance = this;
  attachInterrupt(digitalPinToInterrupt(clkPin), encoderISR, CHANGE);
}

void RotaryEncoder::update() {
  noInterrupts();
  int step = delta;
  delta = 0;
  interrupts();

  if (step) {
    pos += step;
    Serial.printf("Position: %d\n", pos);

    // Call the position change callback if set
    if (onChangeCallback) {
      onChangeCallback(pos);
    }
  }
}

int RotaryEncoder::getPosition() const { return pos; }

void RotaryEncoder::handleInterrupt() {
  int clkNow = digitalRead(clkPin);
  if (clkNow != lastCLK) {
    delta += (digitalRead(dtPin) != clkNow) ? 1 : -1;
    lastCLK = clkNow;
  }
}

void RotaryEncoder::setOnChangeCallback(void (*callback)(int pos)) {
  onChangeCallback = callback;
}

RotaryEncoder* RotaryEncoder::instance = nullptr;

// ==========================
// EncoderButton Implementation
// ==========================

EncoderButton::EncoderButton(uint8_t pin, unsigned long debounceMs)
    : pin(pin),
      debounceMs(debounceMs),
      lastState(HIGH),
      lastChange(0),
      pressed(false) {}

void EncoderButton::begin() { pinMode(pin, INPUT_PULLUP); }

void EncoderButton::update() {
  int reading = digitalRead(pin);

  if (reading != lastState) lastChange = millis();

  if ((millis() - lastChange) > debounceMs) {
    if (reading == LOW && !pressed) {
      pressed = true;
      onPress();
    } else if (reading == HIGH && pressed) {
      pressed = false;
    }
  }

  lastState = reading;
}

void EncoderButton::onPress() {
  Serial.println("Button Pressed");

  // Call the button press callback if set
  if (onPressCallback) {
    onPressCallback();
  }
}

void EncoderButton::setOnPressCallback(void (*callback)()) {
  onPressCallback = callback;
}

// ==========================
// Interrupt Service Routine
// ==========================

void IRAM_ATTR encoderISR() {
  if (RotaryEncoder::instance) RotaryEncoder::instance->handleInterrupt();
}