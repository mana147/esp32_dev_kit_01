#include "Button.h"

void Button::begin()
{
  pinMode(_pin, INPUT_PULLDOWN);
}

void Button::update()
{
  const bool reading = (digitalRead(_pin) == HIGH);
  const uint32_t now = millis();

  if (reading != _lastReading)
  {
    _lastReading = reading;
    _lastChangeMs = now;
  }

  if (now - _lastChangeMs >= _debounceMs && reading != _stable)
  {
    _stable = reading;
    if (_stable)
      _pressedEvent = true;
  }
}

bool Button::pressed()
{
  const bool event = _pressedEvent;
  _pressedEvent = false;
  return event;
}
