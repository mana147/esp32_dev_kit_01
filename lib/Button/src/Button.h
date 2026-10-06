#pragma once
#include <Arduino.h>

// Active-high button with internal pull-down and software debounce.
class Button
{
public:
  Button(uint8_t pin, uint32_t debounceMs) : _pin(pin), _debounceMs(debounceMs) {}

  void begin();
  void update();  // call from loop() before pressed()
  bool pressed(); // true once per press (clears the event); requires update() in loop()
  bool isDown() const { return _stable; }

private:
  uint8_t _pin;
  uint32_t _debounceMs;
  bool _lastReading = false;
  bool _stable = false;
  bool _pressedEvent = false;
  uint32_t _lastChangeMs = 0;
};
