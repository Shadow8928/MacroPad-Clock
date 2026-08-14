#pragma once
#include <Arduino.h>

// Drives the active buzzer through the 2N3904 (GPIO HIGH -> base
// resistor -> transistor conducts -> buzzer sounds). Non-blocking
// beep pattern while any timer alarm is active and not muted.
class Buzzer {
public:
  void begin(uint8_t pin);
  void update(bool alarmActive, bool muted);

private:
  uint8_t _pin = 0;
  bool _on = false;
  unsigned long _lastToggleMs = 0;
};
