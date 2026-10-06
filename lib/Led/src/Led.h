#pragma once
#include <Arduino.h>

// LED active HIGH; gọi begin() trước khi điều khiển.
class Led
{
public:
  explicit Led(uint8_t pin) : _pin(pin) {}

  void begin();
  void set(bool on);
  void toggle();
  bool isOn() const { return _on; }

private:
  uint8_t _pin;
  bool _on = false;
};
